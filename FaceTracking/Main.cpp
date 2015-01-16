
#include <iostream>
#include "PFilter.h"
#include "FaceDetect.h"
#include "CalcLike.h"
#include "MShift.h"
#include "OcvFD.h"
#include "Hough.h"
#include "VgaCap.h"
#include "TemplateMatching.h"

int w = 640;//画面の幅
int h = 480;//画面の高さ
int num = 100;//パーティクルの数
Size blockSize = Size(30,30);//粒子のブロックサイズ（決め打ち）
Size cellSize = Size(5,5);//粒子のブロックサイズ（決め打ち）
Point mousePoint;
bool blockImgDone = true;

void onMouse1 (int event, int x, int y, int flags, void *param = NULL)
{  
	char str[64];
	static int line = 0;
	// マウスイベントを取得 
	switch (event) 
	{
	case CV_EVENT_LBUTTONDOWN:
		sprintf (str, "(%d,%d) %s", x, y, "LBUTTON_DOWN");
		break;

	case CV_EVENT_LBUTTONUP:
		sprintf (str, "(%d,%d) %s", x, y, "LBUTTON_UP");
		mousePoint.x=(x>blockSize.width)?((x<w-blockSize.width)?x:w-blockSize.width):blockSize.width;
		mousePoint.y=(y>blockSize.height)?((y<h-blockSize.height)?y:h-blockSize.height):blockSize.height;
		break;
	} 
}

//指定したサイズに内接した画像にリサイズ
void inscribedResize(Mat src, Mat &dst, Size size, int interpolation = 1){	
	int sub = src.rows - src.cols;
	int eH = (sub > 0)? size.height : src.rows * (double)((double)size.width / src.cols);
	int eW = (sub > 0)? src.cols * (double)((double)size.height / src.rows) : size.width;
	resize(src, dst, Size(eW, eH), 0, 0, interpolation);
}

int main(int argc, char** argv)
{
	Mat blockImg;//追跡用のブロック画像

	//状態変数の上限、下限
	LIMIT upper, lower;
	upper.x=w-blockSize.width/2; upper.y= h-blockSize.height/2; upper.vx=w/2; upper.vy=h/2;
	lower.x=0+blockSize.width/2; lower.y=0+blockSize.height/2; lower.vx=-w/2; lower.vy=-h/2;

	// ノイズの最大値
	NOISE noise;
	noise.x=20; noise.y=20; noise.vx=40; noise.vy=40;

	// パーティクルフィルタ
	PFilter* pf = new PFilter(num, upper, lower, noise);
	// 位置推定の出力用
	Particle *p = new Particle();

	cout<<"pf initialize"<<endl;

	// OpenCV1を使ってUSBカメラからキャプチャする
	/*CvCapture *capture = 0;
	capture = cvCreateCameraCapture(0);
	if( capture == NULL )
	{
	cout<<"not found camera"<<endl;
	return 0;
	}
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, w);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, h);*/

	//OpenCV2を使ってUSBカメラからキャプチャする
	VideoCapture cap(1);
	if(!cap.isOpened()){
		cout<<"not found camera"<<endl;
		return 0;
	}
	cap.set(CV_CAP_PROP_FRAME_WIDTH, w);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, h);
	//cap >> img; // カメラから新しいフレームを取得

	// 動画像保存用構造体
	//CvVideoWriter *vw;
	// ビデオライタ構造体を作成（ 640x480, 10fps, avi(mpeg4) ）
	// vw = cvCreateVideoWriter ("result/cap.avi", CV_FOURCC ('X', 'V', 'I', 'D'), 10, cvSize ((int) w, (int) h));

	//OpenCV2　動画保存
	string outputPath = "result/cap.avi";
	double outputFPS = 30;
	Size outputSize = Size(w, h);
	//VideoWriter writer(outputPath, CV_FOURCC_DEFAULT, outputFPS, outputSize);
	//Mat frame;
	//writer << frame; // フレームを保存
	//if(!writer.isOpened()) return -1;
	//writer.release();//すべてが終了したら解放

	// 観測画像
	//IplImage* img = cvCreateImage(cvSize(w,h), 8, 3);
	Mat img;
	// 結果画像
	//IplImage* dst = cvCreateImage(cvSize(w,h), 8, 3);
	Mat dst;

	// ウィンドウの生成
	//cvNamedWindow("img");
	//cvNamedWindow("dst");

	//img = cvQueryFrame (capture);

	/*#ifdef _DEBUG // 1ステップ目の実行結果を保存する
	cvCopy(img, dst);
	cvSaveImage("img/000.jpg", dst);
	for(int i=0; i<num; i++){
	cvCircle(dst, cvPoint( pf->particles[i]->get_x(), pf->particles[i]->get_y() ),
	2, CV_RGB(0,0,255), CV_FILLED);
	}
	cvSaveImage("img/000-particle.jpg", dst);
	pf->predict();
	cvCopy(img, dst);
	for(int i=0; i<num; i++){
	cvCircle(dst, cvPoint( pf->pre_particles[i]->get_x(), pf->pre_particles[i]->get_y() ),
	2, CV_RGB(0,255,0), CV_FILLED);
	}
	cvSaveImage("img/001-predict.jpg", dst);
	pf->weight(img);
	cvCopy(img, dst);
	for(int i=0; i<num; i++){
	cvCircle(dst, cvPoint( pf->pre_particles[i]->get_x(), pf->pre_particles[i]->get_y() ), 
	1*pf->pre_particles[i]->getWeight()*1000, CV_RGB(0, 255, 255) );
	}
	cvSaveImage("img/002-weight.jpg", dst);
	pf->measure(p);
	cvCopy(img, dst);
	cvCircle(dst, cvPoint( p->get_x(), p->get_y() ),
	10, CV_RGB(255, 120, 120), CV_FILLED);
	cvSaveImage("img/003-measure.jpg", dst);
	pf->resample();
	cvCopy(img, dst);
	for(int i=0; i<num; i++){
	cvCircle(dst, cvPoint( pf->particles[i]->get_x(), pf->particles[i]->get_y() ), 
	2, CV_RGB(0, 0, 255), CV_FILLED);
	}
	cvSaveImage("img/004-resample.jpg", dst);
	#endif*/

	unsigned char key = 0;
	bool particleFlag = true;
	bool measureFlag = true;
	bool loopFlag = true;
	Rect roiRect;
	while(loopFlag){

		//ブロック画像作成フェイズ
		mousePoint.x = 100;
		mousePoint.y = 100;
		Mat templ = imread("result\\model\\test2\\resizeImg90.jpg");
		while(blockImgDone){
			cap >> img; // カメラから新しいフレームを取得
			img = templ;
			roiRect = Rect(Point(mousePoint.x-blockSize.width/2, mousePoint.y-blockSize.height/2),
				Point(mousePoint.x+blockSize.width/2, mousePoint.y+blockSize.height/2));
			img(roiRect).copyTo(blockImg);
			namedWindow("img", CV_WINDOW_AUTOSIZE);
			setMouseCallback("img", onMouse1, "img");
			imshow("img", img);
			imshow("blockIMG",blockImg);
			img.copyTo(dst);
			rectangle(dst,roiRect, CV_RGB(0, 0, 255), 2);
			imshow("img", dst);

			key = waitKey(33);
			if( key == 27 ){
				blockImgDone = false;
				destroyAllWindows();
			}
		}

		//尤度計算テスト
		CalcLike cl = CalcLike(blockImg, blockSize, cellSize);

		cl.print();
		imshow("average",cl.getAverageImg());
		imshow("blockImg",blockImg);

		//顔検出用クラス
		//FD fd;
		//顔検出の成否
		bool faceDetected = false;
		Size faceSize = Size(100,100);
		//ヒストグラム計算用クラス
		calcHSVHist ch = calcHSVHist(blockImg);

		pf->setCL(cl);
		pf->setCH(ch);

		//ch.baseNormHist.show("baseHist");

		Hough hough = Hough();

		//ヒストグラムテスト用
		//Mat srctes = imread("C:\\Users\\ymaday\\Pictures\\tes.jpg");
		//calcHSVHist chtes = calcHSVHist(srctes);
		//chtes.baseNormHist.show("tes");


		//エッジ抽出用
		Mat eSrc[181];
		Mat resizeImg, binImg, sobelImg, laplacianImg, cannyImg, binsobelImg, binlaplacianImg, bincannyImg;

		for(int i = 145; i <= 135; i+=3){
			ostringstream oss;
			oss << i << ".jpg";
			eSrc[i] = imread("result\\model\\kettei2\\" + oss.str(), CV_LOAD_IMAGE_GRAYSCALE);
			Mat reSrc = eSrc[i](Rect(632, 200, 2000, 1500));
			//imshow("resrc", reSrc);
			//指定したサイズにリサイズ
			inscribedResize(reSrc, resizeImg, Size(640, 480), INTER_AREA);
			imwrite("result\\model\\test3\\resizeImg" + oss.str(), resizeImg);
			//Sobel(resizeImg, sobelImg, CV_32F, 1, 1);
			//imwrite("result\\model\\test\\soble" + oss.str(), sobelImg);
			//Laplacian(resizeImg, laplacianImg, CV_32F, 3);
			//imwrite("result\\model\\test\\laplacian" + oss.str(), laplacianImg);
			//Canny(resizeImg, cannyImg, 50, 200, 3);
			//imwrite("result\\model\\test\\canny" + oss.str(), cannyImg);

			//大津の二値化をしてから再度変換
			//cv::threshold(resizeImg, binImg, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
			//cv::threshold(resizeImg, binImg, 85, 255, CV_THRESH_BINARY);
			//imwrite("result\\model\\test\\binary\\resizeImg" + oss.str(), binImg);
			//Sobel(binImg, binsobelImg, CV_32F, 1, 1);
			//imwrite("result\\model\\test\\binary\\soble" + oss.str(), binsobelImg);
			//Laplacian(binImg, binlaplacianImg, CV_32F, 3);
			//imwrite("result\\model\\test\\binary\\laplacian" + oss.str(), binlaplacianImg);
			//Canny(binImg, bincannyImg, 50, 200, 3);
			//imwrite("result\\model\\test\\binary\\canny" + oss.str(), bincannyImg);
		}

		/*bool showLoop = true;
		Mat glasses[181];
		for(int i = 0;i<=180;i+=3){
			ostringstream oss;
			oss << i << ".jpg";
			Mat glass = imread("result\\model\\test\\resizeImg" + oss.str(), CV_LOAD_IMAGE_GRAYSCALE);
			Rect roiG = Rect(50,150,glass.cols-100,150);
			glasses[i] = glass(roiG);
		}
		int count = 0;
		while(showLoop){
			imshow("glasses", glasses[count]);
			count+=3;
			if(count == 180) count = 0;
			key = waitKey(33);
			if(key == 32) showLoop = false;
		}
		//eSrc = imread("data\\DSCN0532s.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		*/

		Mat gtemp = imread("result\\model\\test\\binary\\resizeImg90.jpg");
		//テンプレートマッチング用
		TemplateMatching tempMatch;
		tempMatch.match(cap, gtemp);
		
		Mat f;
		string csv_path = "data\\data_real_LM_RS.csv";
		Mat temp = imread("result\\model\\test2\\resizeImg90.jpg"),gf;
		bool loopCSV = true;
		while(loopCSV){
			cap >> f;
			cvtColor(f, gf, CV_RGB2GRAY);
			tempMatch.matchCSV(f, csv_path);
			key = waitKey(33);
			if( key == 27 ){
				loopCSV = false;
				destroyAllWindows();
			}
		}

		Mat sgtemp;
		inscribedResize(gtemp, sgtemp, Size(320, 240), INTER_AREA);

		//画像切り取り
		//VgaCap vgaCap;
		//vgaCap.cap(cap);

		//SIFTやSURFとかの特徴点検出
		OcvFD ofd = OcvFD("SURF", "SURF", "BruteForce");
		bool fdloop = true;
		while(fdloop){
			cap >> img;
			ofd.matching(sgtemp, img, true);
			key = waitKey(33);
			switch(key){
			case 27:
				fdloop = false;
				destroyAllWindows();
				break;
			}
		}
		//ofd.matching(blockImg, img, "SIFT", "SIFT", "BruteForce",0);
		//ofd.matching(img, img, "BRISK", "BRISK", "BruteForce");
		//ofd.matching(img, img, "ORB", "ORB", "BruteForce");

		const int TRACKING_PARTICLE = 1;
		const int TRACKING_MEANSHIFT = 2;
		int trackingType = TRACKING_PARTICLE;

		for(;;){
			//img = cvQueryFrame (capture);
			cap >> img; // カメラから新しいフレームを取得
			img.copyTo(dst);

			hough.calc(img);
			hough.show();

			switch(trackingType){
			case TRACKING_PARTICLE://パーティクルフィルタの場合
				//顔検出
				//if(!faceDetected){
				//	faceDetected = fd.detect(img,ch);
				//faceSize = fd.getFaceImage().size();
				//}
				//if(faceDetected){
				pf->predict();  

				pf->weight(img, blockSize, img);

				pf->measure(p);

				pf->resample();

				// パーティクルの表示
				if(particleFlag){
					for(int i=0; i<num; i++){
						circle(dst, cvPoint(pf->particles[i]->get_x(), pf->particles[i]->get_y()), pf->particles[i]->getWeight()*num*5 , CV_RGB(255, 0, 0), CV_FILLED);
						//rectangle(dst,Point( pf->particles[i]->get_x()-blockSize.width/2, pf->particles[i]->get_y()-blockSize.height/2),
						//Point( pf->particles[i]->get_x()+blockSize.width/2, pf->particles[i]->get_y()+blockSize.height/2), CV_RGB(0, 0, 255), 2);
						//cout << pf->particles[i]->getWeight() << endl;
					}
				}
				// 物体位置（パーティクルの重心）推定結果の表示
				if(measureFlag){
					//circle(dst, cvPoint( p->get_x(), p->get_y() ), 10, cl.getCenterColor(), CV_FILLED);
					rectangle(dst,Point( p->get_x()-blockSize.width/2, p->get_y()-blockSize.height/2),
						Point( p->get_x()+blockSize.width/2, p->get_y()+blockSize.height/2), CV_RGB(0, 0, 255), 2);
				}
				//}


				// ビデオに書き出し
				//  cvWriteFrame (vw, dst);

				//imshow("img", img);
				imshow("dst", dst);
				break;
			case TRACKING_MEANSHIFT://meanshift追跡の場合
				MShift ms = MShift();
				dst = ms.run(img, roiRect);
				imshow("dst", dst);
				break;
			}

			key = cvWaitKey(33);
			if( key == 27 ){
				loopFlag = false;//ESCで終了
				break;
			}
			else if((char)key == 'q'){
				blockImgDone = true;//Qでブロック画像選択に戻る
				break;
			}
			switch( (char)key ){
			case 'p':
				particleFlag = !particleFlag;
				break;
			case 'm':
				measureFlag = !measureFlag;
				break;
			default:
				break;
			}

		}
		//delete &cl;
	}
	cout << "test end." << endl;
	return 0;
}