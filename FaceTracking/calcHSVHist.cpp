#include "calcHSVHist.h"

const int divisor = 20;

calcHSVHist::calcHSVHist(void)
{
	isBase = false;
}

calcHSVHist::calcHSVHist(Mat _baseImg)
{
	Mat srcHSV;
	baseImg = _baseImg;
	maxBin = 13;//256の20分の1の12.8の値を繰り上げ
	//maxBin = 18;//256の15分の1の17.0~の値を繰り上げ
	//maxBin = 26;//256の10分の1の25.6の値を繰り上げ
	//maxBin = 52;//256の5分の1の51.2の値を繰り上げ
	maxDim = 3;
	cvtColor(baseImg, srcHSV, CV_BGR2HSV);
	baseHist = Histogram(maxBin, maxDim);
	baseNormHist = Histogram(maxBin, maxDim);

	for ( int y=0; y<srcHSV.rows; ++y ) {
        for ( int x=0; x<srcHSV.cols; ++x ) {
            cv::Vec3b &v = srcHSV.at<cv::Vec3b>(y,x);
			baseHist.increment(1,v[0]/divisor); // H
            baseHist.increment(2,v[1]/divisor); // S 
            baseHist.increment(3,v[2]/divisor); // V 
        }
    }
	calcNormHist(baseHist, baseNormHist);
	//baseHist.show("h");
	//baseNormHist.show("");
	isBase = true;
}

calcHSVHist::~calcHSVHist(void)
{
}


Histogram calcHSVHist::hsvHist(Mat src)
{
	Mat srcHSV;
	cvtColor(src,srcHSV,CV_BGR2HSV);
	Histogram hsvHist(maxBin,maxDim);

	for ( int y=0; y<srcHSV.rows; ++y ) {
        for ( int x=0; x<srcHSV.cols; ++x ) {
            cv::Vec3b &v = srcHSV.at<cv::Vec3b>(y,x);
			hsvHist.increment(1,v[0]/divisor); // H
            hsvHist.increment(2,v[1]/divisor); // S 
            hsvHist.increment(3,v[2]/divisor); // V 
        }
    }
	return hsvHist;
}

void calcHSVHist::hsvBaseHist(Mat _baseImg)
{
	Mat srcHSV;
	_baseImg.copyTo(baseImg);
	cvtColor(baseImg,srcHSV,CV_BGR2HSV);

	for ( int y=0; y<srcHSV.rows; ++y ) {
        for ( int x=0; x<srcHSV.cols; ++x ) {
            cv::Vec3b &v = srcHSV.at<cv::Vec3b>(y,x);
			baseHist.increment(1,v[0]/divisor); // H
            baseHist.increment(2,v[1]/divisor); // S 
            baseHist.increment(3,v[2]/divisor); // V 
        }
    }
	calcNormHist(baseHist, baseNormHist);
	//srcHSV.release;
}


double  calcHSVHist::calcLikelihood(Histogram srcHist)//バタチャリア距離で尤度を計算
{
	Histogram srcNormHist = Histogram(maxBin, maxDim);
	calcNormHist(srcHist, srcNormHist);
	double like_h = .0, like_s = .0, like_v = .0, like_hsv = .0, like_hv = .0;
	//double a,b,ab,ab2,rab,rab2,rab3;

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
	//各HSVで負の対数をとる
	//like_h = -logf(like_h);
	//like_s = -logf(like_s);
	//like_v = -logf(like_v);

	//求めた類似度を二乗した和のルートを求める
	//like_hsv = std::sqrt(std::pow(like_h,like_h)+std::pow(like_s,like_s)+std::pow(like_v,like_v));
	
	//求めた類似度の平均をとる
	like_hsv = (like_h + like_s + like_v)/ 3.0;
	//like_hv = (like_h + like_v)/2;
	
	//cout << like_h << "," << like_s << "," << like_v << endl;
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
	//int sumpixs = baseImg.rows*baseImg.cols;

	//cout << sum_h << sum_s << sum_v << endl;
	//cout << sumpixs << endl;

	//求めた合計で各ビンを割る
	for (int i = 0; i<srcHist.getBins(); ++i) {
		dstHist.setVal(1, i, srcHist.getVal(1, i) / sum_h);
		dstHist.setVal(2, i, srcHist.getVal(2, i) / sum_s);
		dstHist.setVal(3, i, srcHist.getVal(3, i) / sum_v);
		//cout << srcHist.getVal(1, i) / sum_h << endl;
		//dstHist.setVal(1, i, srcHist.getVal(1, i) / sumpixs);
		//dstHist.setVal(2, i, srcHist.getVal(2, i) / sumpixs);
		//dstHist.setVal(3, i, srcHist.getVal(3, i) / sumpixs);
		//cout << srcHist.getVal(1, i) / sumpixs << endl;
    }
}

bool calcHSVHist::isBaseHist(){
	return isBase;
}
