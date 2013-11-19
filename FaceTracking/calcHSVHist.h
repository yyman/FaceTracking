#pragma once
#ifndef CALC_HSV_HIST_H_
#define CALC_HSV_HIST_H_


//'fopen' の宣言を確認してください。回避用
#pragma warning( disable : 4996 )

#include <vector>

#include <stdio.h>
#include <math.h>
#include <opencv\cv.h>
#include <opencv2\opencv.hpp>
#include <opencv2\calib3d\calib3d.hpp>

#include "Histogram.h"

#ifdef _DEBUG
//Debugモードの場合
#pragma comment(lib,"opencv_core245d.lib")
#pragma comment(lib,"opencv_imgproc245d.lib")
#pragma comment(lib,"opencv_highgui245d.lib")
#pragma comment(lib,"opencv_objdetect245d.lib")
#pragma comment(lib,"opencv_contrib245d.lib")
#pragma comment(lib,"opencv_features2d245d.lib")
#pragma comment(lib,"opencv_flann245d.lib")
#pragma comment(lib,"opencv_gpu245d.lib")
#pragma comment(lib,"opencv_legacy245d.lib")
#pragma comment(lib,"opencv_ts245d.lib")
#pragma comment(lib,"opencv_video245d.lib")
#else
//Releaseモードの場合
#pragma comment(lib,"opencv_core245.lib")
#pragma comment(lib,"opencv_imgproc245.lib")
#pragma comment(lib,"opencv_highgui245.lib")
#pragma comment(lib,"opencv_objdetect245.lib")
#pragma comment(lib,"opencv_contrib245.lib")
#pragma comment(lib,"opencv_features2d245.lib")
#pragma comment(lib,"opencv_flann245.lib")
#pragma comment(lib,"opencv_gpu245.lib")
#pragma comment(lib,"opencv_legacy245.lib")
#pragma comment(lib,"opencv_ts245.lib")
#pragma comment(lib,"opencv_video245.lib")
#endif

using namespace std;
using namespace cv;


class calcHSVHist
{
public:
	std::vector<Histogram> hists;
	Histogram baseHist;
	Histogram baseNormHist;
	Mat baseImg;
	bool isBase;
	calcHSVHist(void);
	calcHSVHist(Mat baseImg);
	~calcHSVHist(void);
	Histogram hsvHist(Mat src);//HSVヒストグラム作成
	void hsvBaseHist(Mat _baseHist);//ベースHSVヒストグラム作成
	bool isBaseHist();//ベースHSVヒストグラム作成
	double  calcHSVHist::calcLikelihood(Histogram srcHist);//バタチャリア距離を計算
	void calcHSVHist::calcNormHist(Histogram srcHist, Histogram& dstHist);//HSV各ヒストグラムが合計1になるように正規化
};

#endif /*CALC_HSV_HIST_H_*/