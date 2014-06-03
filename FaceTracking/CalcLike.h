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
	Histogram hsvHist(Mat src);//HSV�q�X�g�O�����쐬
	void hsvBaseHist(Mat _baseHist);//�x�[�XHSV�q�X�g�O�����쐬
	bool isBaseHist();//�x�[�XHSV�q�X�g�O�����쐬
	double  CalcLike::calcLikelihood(Histogram srcHist);//�o�^�`�����A�������v�Z
	void CalcLike::calcNormHist(Histogram srcHist, Histogram& dstHist);//HSV�e�q�X�g�O���������v1�ɂȂ�悤�ɐ��K��
};

#endif /*CALC_LIKE_H_*/