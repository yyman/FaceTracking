#include "CalcLike.h"


CalcLike::CalcLike(void)
{
	isBase = false;
}

CalcLike::CalcLike(Mat _src, Size _blockSize)
{
	src = _src;
    blockSize = _blockSize;
	cellSize = Size(5,5);
	
	for(int i = 0;i < cellSize.width*cellSize.height*3; i++){
		cell.push_back(0);
	}
	int cellW = blockSize.width / cellSize.width;//ƒZƒ‹‚Ì•
	int cellH = blockSize.height / cellSize.height;//ƒZƒ‹‚Ì‚‚³
	int wCount = 0;
	int hCount = 0;
	for ( int y=0; y<src.rows; ++y, hCount++) {
		if(hCount <= cellH){
			hCount = 0;
		}
        for ( int x=0; x<src.cols; ++x, wCount++) {
			if(wCount <= cellW){
				wCount = 0;
			}
            cv::Vec3b &v = src.at<cv::Vec3b>(y,x);
			cell[] += v[0]; // B
            cell[] += v[1]; // G 
            cell[] += v[2]; // R 
        }
    }
}


CalcLike::~CalcLike(void)
{
}


Histogram CalcLike::hsvHist(Mat src)
{
	Mat srcHSV;
	int maxBin = 256;
	int maxDim = 3;
	cvtColor(src,srcHSV,CV_BGR2HSV);
	Histogram hsvHist(maxBin,maxDim);

	for ( int y=0; y<srcHSV.rows; ++y ) {
        for ( int x=0; x<srcHSV.cols; ++x ) {
            cv::Vec3b &v = srcHSV.at<cv::Vec3b>(y,x);
			hsvHist.increment(1,v[0]); // H
            hsvHist.increment(2,v[1]); // S 
            hsvHist.increment(3,v[2]); // V 
        }
    }
	return hsvHist;
}