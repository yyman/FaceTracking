#ifndef BLOCK_H_
#define BLOCK_H_

#include <vector>
#include "Define.h"

class Block
{
private:
	Mat src,averageSrc;
	Size blockSize;
	Size cellSize;
	int cellW,cellH;
	cv::Scalar centerColor;
	std::vector<double> cell;

public:
	Block();
	Block(Mat _src, Size _blockSize, Size _cellSize);

	virtual ~Block();

	Mat getSrc();
	int getCellW();
	int getCellH();
	Size getBlockSize();
	Size getCellSize();
	vector<double> getCell();
	cv::Scalar getColor(int x,int y);
	cv::Scalar getCenterColor();

	void set_x(int _x);
	void set_y(int _y);
	void set_vx(int _vx);
	void set_vy(int _vy);
	void setBlockSize();
	void setCellSize();
	void setHist(Block _pHist);
	void calcAverageColor();

	
	Mat getAverageImg();

};

#endif /*BLOCK_H_*/