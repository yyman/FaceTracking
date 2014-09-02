#include "calcHSVHist.h"


calcHSVHist::calcHSVHist(void)
{
	isBase = false;
}

calcHSVHist::calcHSVHist(Mat _baseImg)
{
	Mat srcHSV;
	baseImg = _baseImg;
	maxBin = 26;//256��5����1��51.4�̒l
	maxDim = 3;
	cvtColor(baseImg, srcHSV, CV_BGR2HSV);
	baseHist = Histogram(maxBin, maxDim);
	baseNormHist = Histogram(maxBin, maxDim);

	for ( int y=0; y<srcHSV.rows; ++y ) {
        for ( int x=0; x<srcHSV.cols; ++x ) {
            cv::Vec3b &v = srcHSV.at<cv::Vec3b>(y,x);
			baseHist.increment(1,v[0]/10); // H
            baseHist.increment(2,v[1]/10); // S 
            baseHist.increment(3,v[2]/10); // V 
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
			hsvHist.increment(1,v[0]/10); // H
            hsvHist.increment(2,v[1]/10); // S 
            hsvHist.increment(3,v[2]/10); // V 
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
			baseHist.increment(1,v[0]/10); // H
            baseHist.increment(2,v[1]/10); // S 
            baseHist.increment(3,v[2]/10); // V 
        }
    }
	calcNormHist(baseHist, baseNormHist);
	//srcHSV.release;
}


double  calcHSVHist::calcLikelihood(Histogram srcHist)//�o�^�`�����A�����Ŗޓx���v�Z
{
	Histogram srcNormHist = Histogram(maxBin, maxDim);
	calcNormHist(srcHist, srcNormHist);
	double like_h = .0, like_s = .0, like_v = .0, like_hsv = .0, like_hv = .0;
	//double a,b,ab,ab2,rab,rab2,rab3;

	//�e�r����sqrt(base*hist)�̍��v���ėގ��x�����߂�
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
	//�eHSV�ŕ��̑ΐ����Ƃ�
	//like_h = -logf(like_h);
	//like_s = -logf(like_s);
	//like_v = -logf(like_v);

	//���߂��ގ��x���悵���a�̃��[�g�����߂�
	//like_hsv = std::sqrt(std::pow(like_h,like_h)+std::pow(like_s,like_s)+std::pow(like_v,like_v));
	
	//���߂��ގ��x�̕��ς��Ƃ�
	//like_hsv = like_h + like_s + like_v / 3.0;
	like_hv = (like_h + like_v)/2;
	
		//cout << like_h << "," << like_s << "," << like_v << endl;
	return like_h;
}


void calcHSVHist::calcNormHist(Histogram srcHist, Histogram& dstHist)//HSV�e�q�X�g�O���������v1�ɂȂ�悤�ɐ��K��
{
	double sum_h = 0, sum_s = 0, sum_v = 0;

	//�e�r���̍��v�����߂�
	for ( int i=0; i < srcHist.getBins(); ++i ) {
       sum_h += srcHist.getVal(1,i);
       sum_s += srcHist.getVal(2,i);
       sum_v += srcHist.getVal(3,i);
    }
	//int sumpixs = baseImg.rows*baseImg.cols;

	//cout << sum_h << sum_s << sum_v << endl;
	//cout << sumpixs << endl;

	//���߂����v�Ŋe�r��������
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
