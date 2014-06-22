#pragma once
#ifndef CALC_LIKE_H_
#define CALC_LIKE_H_

#include "Define.h"
#include "Block.h"

class CalcLike
{
private:
	Mat src,averageSrc;
	Block base;
	Size blockSize;
	Size cellSize;
	int cellW,cellH;
	cv::Scalar centerColor;
	std::vector<double> cell;

public:
	CalcLike();
	CalcLike(Mat _src, Size _blockSize, Size _cellSize);
	~CalcLike(void);
	double CalcLike::calcLikelihood(Mat _src, int x, int y, Size _blockSize);
	double CalcLike::calc();
	void CalcLike::print();
	cv::Scalar CalcLike::getCenterColor();
	void CalcLike::calcAverageColor();
	cv::Scalar CalcLike::getColor(int x,int y);
	Mat CalcLike::getBlockImg();
	Mat CalcLike::getAverageImg();
};

#endif /*CALC_LIKE_H_*/