#pragma once
#ifndef CALC_HSV_HIST_H_
#define CALC_HSV_HIST_H_

#include "Define.h"
#include "Histogram.h"

class calcHSVHist
{
private:
	int maxBin;
	int maxDim;
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