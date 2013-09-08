
#include "PFilter.h"
#include "Histogram.h"


Histogram::Histogram(void)
{
	bins = 256;
	dimension = 3;
	//Bin数×次元数の領域確保　０で初期化
	for(int i = 0;i < bins*dimension; i++){
		val.push_back(0);
	}
	calcMaxMinBin();
}


Histogram::Histogram(int b, int d)
{
	bins = b;
	dimension = d;
	//Bin数×次元数の領域確保　０で初期化
	for(int i = 0;i < b*d; i++){
		val.push_back(0);
	}
	calcMaxMinBin();
}


Histogram::~Histogram(void)
{
}


int Histogram::getVal(int dim, int x)
{
	int binVal = val[bins*(dim-1) + x - 1];
	printf_s("%d",binVal);
	return binVal;
}


void Histogram::setVal(int b, int v)
{
	vector<int>::iterator it = val.begin();  // イテレータのインスタンス化
	for(int i=0;i<b;i++){
		it++;
	}
	val[b] = v;
}


void Histogram::increment(int dim, int bin)
{
	//cout<<"dim"<<dim<<" bin"<<bin<<endl;
	val[bins*(dim-1) + bin]++;
}


void Histogram::show()
{
	//cout<<maxBin<<endl;
	Mat dst(cv::Size( bins*dimension, 256), CV_8UC3, cv::Scalar::all(255));
    Scalar colorH = cv::Scalar::all(100);//binの色
    Scalar colorS = cv::Scalar::all(50);//binの色
    Scalar colorV = cv::Scalar::all(0);//binの色
	int normVal;
	int norm = 256/((val[maxBin]>0)?val[maxBin]:256);
	for ( int x=0; x<dst.cols; ++x ) {
		Scalar color;
		if(x < bins) color = colorH;
		else if(x >= bins && x < bins*2) color = colorS;
		else color = colorV;
		normVal = val[x]*norm;
		cv::rectangle(dst,
			cv::Point(x, dst.rows),
			cv::Point(x+1, dst.rows-normVal),
			color, -1);
	}
	imshow("hist",dst);
}


void Histogram::calcMaxMinBin()
{
	maxBin = 0;
	minBin = 0;
	for(int i=0; i<bins*dimension;i++){
		if(val[maxBin] < val[i]) maxBin = i;
		if(val[maxBin] > val[i]) minBin = i;
	}
}