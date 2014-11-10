#ifndef HOUGH_H_
#define HOUGH_H_

#include <vector>
#include "Define.h"

class Hough
{
private:
	Mat src, dst, GrayImg;

public:
	Hough();
	~Hough();

	void Hough::calc(Mat src);
	void Hough::show();
};

#endif /*HOUGH_H_*/