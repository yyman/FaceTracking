#include "calcHSVHist.h"


calcHSVHist::calcHSVHist(void)
{
}

calcHSVHist::calcHSVHist(Mat roi)
{
	Mat srcHSV;
	int maxBin = 256;
	int maxDim = 3;
	cvtColor(roi,srcHSV,CV_BGR2HSV);

	for ( int y=0; y<srcHSV.rows; ++y ) {
        for ( int x=0; x<srcHSV.cols; ++x ) {
            cv::Vec3b &v = srcHSV.at<cv::Vec3b>(y,x);
			baseHist.increment(1,v[0]); // H
            baseHist.increment(2,v[1]); // S 
            baseHist.increment(3,v[2]); // V 
        }
    }
}


calcHSVHist::~calcHSVHist(void)
{
}


void calcHSVHist::hsvHist(Mat src)
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
