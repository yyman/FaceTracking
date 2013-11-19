#pragma once
#ifndef CALC_HSV_HIST_H_
#define CALC_HSV_HIST_H_


//'fopen' �̐錾���m�F���Ă��������B���p
#pragma warning( disable : 4996 )

#include <vector>

#include <stdio.h>
#include <math.h>
#include <opencv\cv.h>
#include <opencv2\opencv.hpp>
#include <opencv2\calib3d\calib3d.hpp>

#include "Histogram.h"

#ifdef _DEBUG
//Debug���[�h�̏ꍇ
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
//Release���[�h�̏ꍇ
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
	Histogram hsvHist(Mat src);//HSV�q�X�g�O�����쐬
	void hsvBaseHist(Mat _baseHist);//�x�[�XHSV�q�X�g�O�����쐬
	bool isBaseHist();//�x�[�XHSV�q�X�g�O�����쐬
	double  calcHSVHist::calcLikelihood(Histogram srcHist);//�o�^�`�����A�������v�Z
	void calcHSVHist::calcNormHist(Histogram srcHist, Histogram& dstHist);//HSV�e�q�X�g�O���������v1�ɂȂ�悤�ɐ��K��
};

#endif /*CALC_HSV_HIST_H_*/