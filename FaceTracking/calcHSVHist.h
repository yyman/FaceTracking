#pragma once
#ifndef CALC_HSV_HIST_H_
#define CALC_HSV_HIST_H_

#include "PFilter.h"
#include "Histogram.h"

class calcHSVHist
{
public:
	vector<Histogram> hist;
	Histogram baseHist;
	calcHSVHist(void);
	calcHSVHist(Mat roi);
	~calcHSVHist(void);
	void hsvHist(Mat src);//HSVƒqƒXƒgƒOƒ‰ƒ€ì¬
};

#endif /*CALC_HSV_HIST_H_*/