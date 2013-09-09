#pragma once
#ifndef CALC_HSV_HIST_H_
#define CALC_HSV_HIST_H_

#include "PFilter.h"
#include "Histogram.h"

class calcHSVHist
{
public:
	std::vector<Histogram> hists;
	Histogram baseHist;
	Histogram baseNormHist;
	Mat baseImg;
	calcHSVHist(void);
	calcHSVHist(Mat baseImg);
	~calcHSVHist(void);
	Histogram hsvHist(Mat src);//HSV�q�X�g�O�����쐬
	void hsvBaseHist(Mat _baseHist);//�x�[�XHSV�q�X�g�O�����쐬
	double  calcHSVHist::calcLikelihood(Histogram srcHist);//�o�^�`�����A�������v�Z
	void calcHSVHist::calcNormHist(Histogram srcHist, Histogram& dstHist);//HSV�e�q�X�g�O���������v1�ɂȂ�悤�ɐ��K��
};

#endif /*CALC_HSV_HIST_H_*/