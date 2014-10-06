#ifndef MSHIFT_H_
#define MSHIFT_H_

#include <vector>
#include "Histogram.h"
#include "Block.h"

class MShift
{
private:
	int x,y,vx,vy;
	double weight;
	Mat image;
	Rect selection;
	Rect trackWindow;
	int hsize;
	int matchesNum;
	Mat frame, hsv, hue, hist, mask, backproj;

public:
	MShift();

	Histogram pHist;
	Block pBlock;

	~MShift();

	int get_x();
	int get_y();
	int get_vx();
	int get_vy();
	double getWeight();
	Histogram getHist();
	Block getBlock();
	Mat run(Mat src, Rect rect);

	void set_x(int _x);
	void set_y(int _y);
	void set_vx(int _vx);
	void set_vy(int _vy);
	void setWeight(double _w);
	void setHist(Histogram _pHist);
	void setBlock(Block _pBlock);

};

#endif /*MSHIFT_H_*/