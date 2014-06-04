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
	int cellW = blockSize.width / cellSize.width;//�Z���̕�
	int cellH = blockSize.height / cellSize.height;//�Z���̍���
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

double CalcLike::calcLikelihood(Mat _src, int x, int y, Size _blockSize){
	double result =0.0;
	Mat img = _src;

	// RGB �F��Ԃ̏ꍇ
	unsigned char b, g, r;
	b = img.data[img.step*y + x*3];     // B
	g = img.data[img.step*y + x*3 + 1]; // G
	r = img.data[img.step*y + x*3 + 2]; // R

	double dist = 0.0, sigma = 50.0;  
	// �ԐF�炵�������[�N���b�h�����Ƃ��ċ��߂�
	dist = sqrt( b*b + g*g + (255-r)*(255-r));
	//cout<<dist<<endl;
	// ����(dist)�𕽋ρAsigma�𕪎U�Ƃ��Ď��A���K���z��ޓx�֐��Ƃ���
	result = 1.0 / (sqrt(2.0*CV_PI)*sigma) * expf(-dist*dist/(2.0*sigma*sigma));
}

double CalcLike::calc()
{
	return 0;
}