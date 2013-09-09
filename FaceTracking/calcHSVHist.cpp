#include "calcHSVHist.h"


calcHSVHist::calcHSVHist(void)
{
}

calcHSVHist::calcHSVHist(Mat _baseImg)
{
	Mat srcHSV;
	baseImg = _baseImg;
	int maxBin = 256;
	int maxDim = 3;
	cvtColor(baseImg,srcHSV,CV_BGR2HSV);

	for ( int y=0; y<srcHSV.rows; ++y ) {
        for ( int x=0; x<srcHSV.cols; ++x ) {
            cv::Vec3b &v = srcHSV.at<cv::Vec3b>(y,x);
			baseHist.increment(1,v[0]); // H
            baseHist.increment(2,v[1]); // S 
            baseHist.increment(3,v[2]); // V 
        }
    }
	calcNormHist(baseHist, baseNormHist);
}


calcHSVHist::~calcHSVHist(void)
{
}


Histogram calcHSVHist::hsvHist(Mat src)
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

void calcHSVHist::hsvBaseHist(Mat _baseImg)
{
	Mat srcHSV;
	baseImg = _baseImg;
	int maxBin = 256;
	int maxDim = 3;
	cvtColor(baseImg,srcHSV,CV_BGR2HSV);

	for ( int y=0; y<srcHSV.rows; ++y ) {
        for ( int x=0; x<srcHSV.cols; ++x ) {
            cv::Vec3b &v = srcHSV.at<cv::Vec3b>(y,x);
			baseHist.increment(1,v[0]); // H
            baseHist.increment(2,v[1]); // S 
            baseHist.increment(3,v[2]); // V 
        }
    }
	calcNormHist(baseHist, baseNormHist);
}


double  calcHSVHist::calcLikelihood(Histogram srcHist)//バタチャリア距離で尤度を計算
{
	Histogram srcNormHist;
	calcNormHist(srcHist, srcNormHist);
	double like_h = .0, like_s = .0, like_v = .0, like_hsv = .0;
	double a,b,ab,ab2,rab,rab2,rab3;

	//各ビンのsqrt(base*hist)の合計して類似度を求める
	for ( int i=0; i<baseNormHist.getBins(); ++i ) {
		/*
		a = baseNormHist.getVal(1,i);
		b = srcNormHist.getVal(1,i);
		ab = baseNormHist.getVal(1,i)*srcNormHist.getVal(1,i);
		ab2 = a*b;
		rab = std::sqrt(baseNormHist.getVal(1,i)*srcNormHist.getVal(1,i));
		rab2 = std::sqrt(ab);
		rab3 = std::sqrt(ab2);*/
       like_h += std::sqrt(baseNormHist.getVal(1,i)*srcNormHist.getVal(1,i));
       like_s += std::sqrt(baseNormHist.getVal(2,i)*srcNormHist.getVal(2,i));
       like_v += std::sqrt(baseNormHist.getVal(3,i)*srcNormHist.getVal(3,i));
    }
	
	//求めた類似度を二乗した和のルートを求める
	like_hsv = std::sqrt(std::pow(like_h,like_h)+std::pow(like_s,like_s)+std::pow(like_v,like_v));
	
		cout << like_h << like_s << like_v << endl;
	return like_hsv;
}


void calcHSVHist::calcNormHist(Histogram srcHist, Histogram& dstHist)//HSV各ヒストグラムが合計1になるように正規化
{
	double sum_h = 0, sum_s = 0, sum_v = 0;

	//各ビンの合計を求める
	for ( int i=0; i < srcHist.getBins(); ++i ) {
       sum_h += srcHist.getVal(1,i);
       sum_s += srcHist.getVal(2,i);
       sum_v += srcHist.getVal(3,i);
    }
		
	//cout << sum_h << sum_s << sum_v << endl;

	//求めた合計で各ビンを割る
	for ( int i=0; i<baseHist.getBins(); ++i ) {
       dstHist.setVal(1,i,srcHist.getVal(1,i) / sum_h);
       dstHist.setVal(2,i,srcHist.getVal(2,i) / sum_s);
       dstHist.setVal(3,i,srcHist.getVal(3,i) / sum_v);
    }
}
