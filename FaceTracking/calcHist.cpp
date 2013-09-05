#include "calcHist.h"


calcHist::calcHist(void)
{
}


calcHist::~calcHist(void)
{
}


void calcHist::hsvHist(Mat src)
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
}
