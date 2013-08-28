
#include <iostream>
#include "PFilter.h"
#include "FaceDetect.h"


string cascadeName = "data\\haarcascade_frontalface_alt.xml";//学習済み検出器
CascadeClassifier cascade;
Mat faceImage;

FD::FD()    //コンストラクタの定義
{
	//初期読み込み
	// 学習済み検出器の読み込み
	cout<<"検出器読み込み開始"<<endl;
    if(!cascade.load(cascadeName)){
		cout<<"検出器が読み込めませんでした"<<endl;
	}
    cout << "コンストラクタ呼び出し＼n";
}

FD::~FD()    //デストラクタの定義
{
	grayImage.release();
    cout << "FDデストラクタ呼び出し＼n";
}

void FD::detect(Mat src)
{
	//顔検出

    cvtColor(src, grayImage, CV_RGB2GRAY);
	// 顔検出を実行
    vector<Rect> faces;
    cascade.detectMultiScale(
        grayImage,   // 画像
        faces,       // 出力される矩形
        1.1,         // 縮小用のスケール
        2,           // 最小の投票数
        CV_HAAR_SCALE_IMAGE,  // フラグ
		cv::Size(30, 30) // 最小の矩形
    );
    // 矩形を描く
    for (vector<Rect>::iterator iter = faces.begin(); iter != faces.end(); iter ++) {
		faceImage = src(*iter);
		imshow("face", faceImage);
	}
}
