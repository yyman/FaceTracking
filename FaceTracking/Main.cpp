
#include <iostream>
#include "PFilter.h"
#include "FaceDetect.h"
#include "CalcLike.h"
#include "MShift.h"
#include "OcvFD.h"
#include "Hough.h"

int w = 640;//画面の幅
int h = 480;//画面の高さ
int num = 100;//パーティクルの数
Size blockSize = Size(100,100);//粒子のブロックサイズ（決め打ち）
Size cellSize = Size(5,5);//粒子のブロックサイズ（決め打ち）
Point mousePoint;
bool blockImgDone = true;

void onMouse (int event, int x, int y, int flags, void *param = NULL)
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
	VideoCapture cap(0);
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
		while(blockImgDone){
			cap >> img; // カメラから新しいフレームを取得
			roiRect = Rect(Point(mousePoint.x-blockSize.width/2, mousePoint.y-blockSize.height/2),
				Point(mousePoint.x+blockSize.width/2, mousePoint.y+blockSize.height/2));
			img(roiRect).copyTo(blockImg);
			namedWindow("img", CV_WINDOW_AUTOSIZE);
			setMouseCallback("img", onMouse, "img");
			//imshow("img", img);
			imshow("blockIMG",blockImg);
			img.copyTo(dst);
			rectangle(dst,roiRect, CV_RGB(0, 0, 255), 2);
			imshow("img", dst);

			key = cvWaitKey(33);
			if( key == 27 ){
				blockImgDone = false;
			}
		}

		//尤度計算テスト
		CalcLike cl = CalcLike(blockImg, blockSize, cellSize);
		/*OcvFD ofd = OcvFD();
		ofd.matching(blockImg, img, "SIFT", "SIFT", "BruteForce",0);
		ofd.matching(blockImg, img, "SURF", "SURF", "BruteForce");
		ofd.matching(img, img, "BRISK", "BRISK", "BruteForce");
		ofd.matching(img, img, "ORB", "ORB", "BruteForce");*/

		//cl.print();
		imshow("average",cl.getAverageImg());
		//imshow("blockImg",blockImg);

		//顔検出用クラス
		//FD fd;
		//顔検出の成否
		bool faceDetected = false;
		Size faceSize = Size(100,100);
		//ヒストグラム計算用クラス
		calcHSVHist ch = calcHSVHist(blockImg);

		pf->setCL(cl);
		pf->setCH(ch);

		ch.baseNormHist.show("baseHist");

		Hough hough = Hough();

		//ヒストグラムテスト用
		//Mat srctes = imread("C:\\Users\\ymaday\\Pictures\\tes.jpg");
		//calcHSVHist chtes = calcHSVHist(srctes);
		//chtes.baseNormHist.show("tes");


		Mat capImg, showImg;
		bool caploop = true;
		int i = 45;
		// 矢印キー
		const int CV_WAITKEY_CURSORKEY_TOP    = 2490368;
		const int CV_WAITKEY_CURSORKEY_BOTTOM = 2621440;
		const int CV_WAITKEY_CURSORKEY_RIGHT  = 2555904;
		const int CV_WAITKEY_CURSORKEY_LEFT   = 2424832;
		while(caploop){
			cap >> capImg;
			capImg.copyTo(showImg);
			ostringstream oss;
			oss << i << ".jpg";
			cv::putText(showImg, oss.str(), cv::Point(50,50), cv::FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(0,0,200), 2, CV_AA);
			imshow("capImg", showImg);
			key = waitKey(33);
			switch(key){
				case 27: 
					caploop = false;
					break;
				case 32:
					imwrite("result\\model\\" + oss.str(), capImg);
					break;
				case 'w':
					i += 3;
					cout<<i<<endl;
					break;
				case 's':
					i -= 3;
					cout<<i<<endl;
					break;
				default:
					//cout<<(char)key<<endl;
					break;
			}
		}


		//エッジ抽出用
		Mat eSrc[5];
		Mat resizeImg, binImg, sobelImg, laplacianImg, cannyImg, binsobelImg, binlaplacianImg, bincannyImg;

		for(int i = 0; i < 5; i++){
			ostringstream oss;
			oss << i << ".jpg";
			eSrc[i] = imread("data\\" + oss.str(), CV_LOAD_IMAGE_GRAYSCALE);
			//指定したサイズにリサイズ
			inscribedResize(eSrc[i], resizeImg, Size(640, 480), INTER_AREA);
			imwrite("result\\resizeImg" + oss.str(), resizeImg);
			Sobel(resizeImg, sobelImg, CV_32F, 1, 1);
			imwrite("result\\soble" + oss.str(), sobelImg);
			Laplacian(resizeImg, laplacianImg, CV_32F, 3);
			imwrite("result\\laplacian" + oss.str(), laplacianImg);
			Canny(resizeImg, cannyImg, 50, 200, 3);
			imwrite("result\\canny" + oss.str(), cannyImg);

			//大津の二値化をしてから再度変換
			cv::threshold(resizeImg, binImg, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
			imwrite("result\\binary\\resizeImg" + oss.str(), binImg);
			Sobel(binImg, binsobelImg, CV_32F, 1, 1);
			imwrite("result\\binary\\soble" + oss.str(), binsobelImg);
			Laplacian(binImg, binlaplacianImg, CV_32F, 3);
			imwrite("result\\binary\\laplacian" + oss.str(), binlaplacianImg);
			Canny(binImg, bincannyImg, 50, 200, 3);
			imwrite("result\\binary\\canny" + oss.str(), bincannyImg);
		}
		//eSrc = imread("data\\DSCN0532s.jpg", CV_LOAD_IMAGE_GRAYSCALE);

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