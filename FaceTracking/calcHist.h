#pragma once
#ifndef CALC_HIST_H_
#define CALC_HIST_H_

#include "PFilter.h"
#include "Histogram.h"

class calcHist
{
public:
	vector<Histogram> hist;
	calcHist(void);
	~calcHist(void);
	void hsvHist(Mat src);//HSV�q�X�g�O�����쐬
};

#endif /*CALC_HIST_H_*/