#include "CalcLike.h"

CalcLike::CalcLike()
{	
}

CalcLike::CalcLike(Mat _src, Size _blockSize, Size _cellSize)
{
	_src.copyTo(src);
	blockSize = _blockSize;
	cellSize = _cellSize;
	base = Block(src,_blockSize,_cellSize);
	cell = base.getCell();
	cellW = base.getCellW();
	cellH = base.getCellH();
	centerColor = base.getCenterColor();
}


CalcLike::~CalcLike(void)
{
}

double CalcLike::calcLikelihood(Mat _src, int x, int y, Size _blockSize){
	double result =0.0;
	Mat img = _src;

	// RGB 色空間の場合
	unsigned char b, g, r;
	b = img.data[img.step*y + x*3];     // B
	g = img.data[img.step*y + x*3 + 1]; // G
	r = img.data[img.step*y + x*3 + 2]; // R

	double dist = 0.0, sigma = 50.0;  
	// 赤色らしさをユークリッド距離として求める
	dist = sqrt( b*b + g*g + (255-r)*(255-r));
	//cout<<dist<<endl;
	// 距離(dist)を平均、sigmaを分散として持つ、正規分布を尤度関数とする
	return result = 1.0 / (sqrt(2.0*CV_PI)*sigma) * expf(-dist*dist/(2.0*sigma*sigma));
}

double CalcLike::calc()
{
	return 0;
}

void CalcLike::print(){

	for(int y=0;y<cellSize.height;y++){
		for(int x=0;x<cellSize.width;x++){
			cout
				<<
					cell[x+cellSize.width*3*y+0]//B
				<<","<<
					cell[x+cellSize.width*3*y+1]//G
				<<","<<
					cell[x+cellSize.width*3*y+2]//R
				<<" "<<ends;
		}
		cout<<endl;
	}

}

cv::Scalar CalcLike::getCenterColor(){
	return centerColor;
}

//各セルの色を取得（0始まり）
cv::Scalar CalcLike::getColor(int x,int y){
	cv::Scalar color = base.getColor(x,y);
	return color;
}

Mat CalcLike::getBlockImg(){
	return src;
}

Mat CalcLike::getAverageImg(){
	averageSrc = Mat(Size(blockSize.width,blockSize.height),CV_8UC3);
	int cnt = 0;
	for(int y=0;y<cellSize.height;y++){
		for(int x=0;x<cellSize.width;x++){
			for(int by=y*cellH; by<y*cellH+cellH;by++){
				for(int bx=x*cellW;bx<x*cellW+cellW;bx++){
					cv::Vec3b &v = averageSrc.at<cv::Vec3b>(by,bx);
					v[0] = cell[x*3+cellSize.width*3*y+0];
					v[1] = cell[x*3+cellSize.width*3*y+1];
					v[2] = cell[x*3+cellSize.width*3*y+2];
					cnt++;
				}
			}
		}
	}
	cout<<cnt<<endl;

	return averageSrc;
}