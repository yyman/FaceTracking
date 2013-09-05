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
	void hsvHist(Mat src);//HSVヒストグラム作成
};

#endif /*CALC_HIST_H_*/