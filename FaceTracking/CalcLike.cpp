#include "CalcLike.h"


CalcLike::CalcLike(void)
{
}

CalcLike::CalcLike(Mat _src, Size _blockSize)
{
	src = _src;
	blockSize = _blockSize;
	cellSize = Size(5,5);

	for(int i = 0;i < cellSize.width*cellSize.height*3; i++){
		cell.push_back(0);
	}
	cellW = blockSize.width / cellSize.width;//セルの幅
	cellH = blockSize.height / cellSize.height;//セルの高さ
	int wCount = 0;
	int hCount = 0;
	int wbCount = 0;
	int hbCount = 0;

	int c;
	
	cout<<cellW<<","<<cellH<<endl;
	for ( int y=0; y<src.rows; ++y) {
		if(hCount == cellH){
			hCount = 0;
			hbCount++;
		}
		for ( int x=0; x<src.cols; ++x) {
			if(wCount == cellW){
				wCount = 0;
				wbCount++;
			}
			cv::Vec3b &v = src.at<cv::Vec3b>(y,x);
			cell[wbCount+cellSize.width*hbCount+0] += v[0]; // B
			cell[wbCount+cellSize.width*hbCount+1] += v[1]; // G 
			cell[wbCount+cellSize.width*hbCount+2] += v[2]; // R 
			//cout<<"wb"<<wbCount<<endl;
			wCount++;
			c++;
		}
		wbCount = 0;
		hCount++;
	}
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
	result = 1.0 / (sqrt(2.0*CV_PI)*sigma) * expf(-dist*dist/(2.0*sigma*sigma));
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
					cell[x+cellSize.width*y+0]/(cellW*cellH) //B
				<<","<<
					cell[x+cellSize.width*y+1]/(cellW*cellH) //G
				<<","<<
					cell[x+cellSize.width*y+2]/(cellW*cellH) //R
				<<" "<<ends;
		}
		cout<<endl;
	}

}