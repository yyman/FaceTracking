
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

void FD::detect(Mat src, calcHSVHist ch)
{
	//顔検出
	//グレースケール変換
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
    // 検出矩形に対し処理
    for (vector<Rect>::iterator iter = faces.begin(); iter != faces.end(); iter ++) {
		faceImage = src(*iter);
		imshow("face", faceImage);
		ch.hsvBaseHist(faceImage);
		double l = ch.calcLikelihood(ch.hsvHist(faceImage));
		cout << l << endl;
		ch.baseHist.show("hist1");
		ch.hsvHist(faceImage).show("hist2");
	}
}
