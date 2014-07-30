
#include <iostream>
#include "PFilter.h"
#include "FaceDetect.h"
#include "CalcLike.h"

int w = 640;//画面の幅
int h = 480;//画面の高さ
int num = 150;//パーティクルの数
Size blockSize = Size(50,50);//粒子のブロックサイズ（決め打ち）
Size cellSize = Size(5,5);//粒子のブロックサイズ（決め打ち）
Point mousePoint;
bool blockImgDone = true;

void onMouse (int event, int x, int y, int flags, void *param = NULL)
{  char str[64];
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
	VideoWriter writer(outputPath, CV_FOURCC_DEFAULT, outputFPS, outputSize);
	//Mat frame;
	//writer << frame; // フレームを保存
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
	while(loopFlag){

		//ブロック画像作成フェイズ
		mousePoint.x = 100;
		mousePoint.y = 100;
		while(blockImgDone){
			cap >> img; // カメラから新しいフレームを取得
			Rect roiRect = Rect(Point(mousePoint.x-blockSize.width/2, mousePoint.y-blockSize.height/2),
				Point(mousePoint.x+blockSize.width/2, mousePoint.y+blockSize.height/2));
			img(roiRect).copyTo(blockImg);
			namedWindow("img", CV_WINDOW_AUTOSIZE);
			setMouseCallback("img", onMouse, "img");
			imshow("img", img);
			imshow("blockIMG",blockImg);
			img.copyTo(dst);
			rectangle(dst,roiRect, CV_RGB(0, 0, 255), 2);
			imshow("rect", dst);

			key = cvWaitKey(33);
			if( key == 27 ){
				blockImgDone = false;
			}
		}

		//尤度計算テスト
		CalcLike cl = CalcLike(blockImg, blockSize, cellSize);

		cl.print();
		imshow("average",cl.getAverageImg());
		//imshow("blockImg",blockImg);

		//顔検出用クラス
		FD fd;
		//顔検出の成否
		bool faceDetected = false;
		Size faceSize = Size(100,100);
		//ヒストグラム計算用クラス
		calcHSVHist ch = calcHSVHist(blockImg);
		
		pf->setCL(cl);
		pf->setCH(ch);

		ch.baseNormHist.show("baseHist");

		for(;;){
			//img = cvQueryFrame (capture);
			cap >> img; // カメラから新しいフレームを取得
			dst = img;

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
					circle(dst, cvPoint( pf->particles[i]->get_x(), pf->particles[i]->get_y() ), 1, CV_RGB(255, 0, 0), CV_FILLED);
					//rectangle(dst,Point( pf->particles[i]->get_x()-blockSize.width/2, pf->particles[i]->get_y()-blockSize.height/2),
						//Point( pf->particles[i]->get_x()+blockSize.width/2, pf->particles[i]->get_y()+blockSize.height/2), CV_RGB(0, 0, 255), 2);
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

			imshow("img", img);
			imshow("dst", dst);

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