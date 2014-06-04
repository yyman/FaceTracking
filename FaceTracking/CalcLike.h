#pragma once
#ifndef CALC_LIKE_H_
#define CALC_LIKE_H_

#include "Define.h"

class CalcLike
{
public:
	Mat src,dst;
	Size blockSize;
	Size cellSize;
	std::vector<double> cell;
	CalcLike(void);
	CalcLike(Mat _src, Size _blockSize);
	~CalcLike(void);
	double  CalcLike::calcLikelihood(Mat _src, int x, int y, Size _blockSize);
	double  CalcLike::calc();
};

#endif /*CALC_LIKE_H_*/