#pragma once
#ifndef CALC_LIKE_H_
#define CALC_LIKE_H_

#include "Define.h"

class CalcLike
{
public:
	std::vector<Histogram> hists;
	Histogram baseHist;
	Histogram baseNormHist;
	Mat baseImg;
	bool isBase;
	CalcLike(void);
	CalcLike(Mat baseImg);
	~CalcLike(void);
	Histogram hsvHist(Mat src);//HSVヒストグラム作成
	void hsvBaseHist(Mat _baseHist);//ベースHSVヒストグラム作成
	bool isBaseHist();//ベースHSVヒストグラム作成
	double  CalcLike::calcLikelihood(Histogram srcHist);//バタチャリア距離を計算
	void CalcLike::calcNormHist(Histogram srcHist, Histogram& dstHist);//HSV各ヒストグラムが合計1になるように正規化
};

#endif /*CALC_LIKE_H_*/