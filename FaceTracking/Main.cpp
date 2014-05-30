
#include <iostream>
#include "PFilter.h"
#include "FaceDetect.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;

int main(int argc, char** argv)
{
	const int w = 640;//画面の幅
	const int h = 480;//画面の高さ
	const int num = 30;//パーティクルの数

	//状態変数の上限、下限
	LIMIT upper, lower;
	upper.x=w; upper.y= h; upper.vx=w/2; upper.vy=h/2;
	lower.x=0; lower.y=0; lower.vx=-w/2; lower.vy=-h/2;

	// ノイズの最大値
	NOISE noise;
	noise.x=50; noise.y=50; noise.vx=100; noise.vy=100;

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

	unsigned char key =0;
	bool particleFlag = true;
	bool measureFlag = true;

	//顔検出用クラス
	FD fd;
	//顔検出の成否
	bool faceDetected = false;
	Size faceSize = Size(100,100);
	//ヒストグラム計算用クラス
	calcHSVHist ch;

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

			pf->weight(img, faceSize, img);

			pf->measure(p);

			pf->resample();

			// パーティクルの表示
			if(particleFlag){
				for(int i=0; i<num; i++){
					circle(dst, cvPoint( pf->particles[i]->get_x(), pf->particles[i]->get_y() ), 
						2, CV_RGB(0, 0, 255), CV_FILLED);
				}
			}
			// 物体位置（パーティクルの重心）推定結果の表示
			if(measureFlag){
				circle(dst, cvPoint( p->get_x(), p->get_y() ), 10, CV_RGB(255,20,20), CV_FILLED);
			}
		//}


		// ビデオに書き出し
		//  cvWriteFrame (vw, dst);

		imshow("img", img);
		imshow("dst", dst);

		key = cvWaitKey(33);
		if( key == 27 ){
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
	cout << "test end." << endl;
	return 0;
}