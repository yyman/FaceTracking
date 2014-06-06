#include "CalcLike.h"


CalcLike::CalcLike(void)
{
}

CalcLike::CalcLike(Mat _src, Size _blockSize)
{
	src = _src;
	//cvtColor(_src,src,CV_BGR2HSV);
	blockSize = _blockSize;
	cellSize = Size(5,5);//奇数のみ

	for(int i = 0;i < cellSize.width*cellSize.height*3; i++){
		cell.push_back(0);
	}
	cellW = blockSize.width / cellSize.width;//セルの幅
	cellH = blockSize.height / cellSize.height;//セルの高さ
	int wCount = 0;
	int hCount = 0;
	int wbCount = 0;
	int hbCount = 0;

	int cnt=0;
	
	cout<<cellW<<","<<cellH<<endl;
	for ( int y=0; y<src.rows; ++y) {
		if(hCount == cellH){
			hCount = 0;
			hbCount++;
		}
		for ( int x=0; x<src.cols; ++x) {
			cv::Vec3b &v = src.at<cv::Vec3b>(y,x);
			cell[wbCount*3+cellSize.width*3*hbCount+0] += v[0]; // B
			cell[wbCount*3+cellSize.width*3*hbCount+1] += v[1]; // G 
			cell[wbCount*3+cellSize.width*3*hbCount+2] += v[2]; // R 
			//cout<<"wb"<<wbCount<<",hb"<<hbCount<<",cnt"<<wbCount*3+cellSize.width*3*hbCount<<endl;
			wCount++;
			if(wCount == cellW){
				wCount = 0;
				wbCount++;
			}
			cnt++;
		}
		wbCount = 0;
		hCount++;
	}
	calcAverageColor();

	centerColor = getColor(int(cellSize.width/2),int(cellSize.height/2));
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

void CalcLike::calcAverageColor(){
	for(int y=0;y<cellSize.height;y++){
		for(int x=0;x<cellSize.width;x++){
			cell[x*3+cellSize.width*3*y+0] = cell[x*3+cellSize.width*3*y+0]/(cellW*cellH); //B
			cell[x*3+cellSize.width*3*y+1] = cell[x*3+cellSize.width*3*y+1]/(cellW*cellH); //G
			cell[x*3+cellSize.width*3*y+2] = cell[x*3+cellSize.width*3*y+2]/(cellW*cellH); //R
		}
	}
}

//各セルの色を取得（0始まり）
cv::Scalar CalcLike::getColor(int x,int y){
	cv::Scalar color = cv::Scalar(cell[x*3+cellSize.width*3*y+0],
							cell[x*3+cellSize.width*3*y+1],
							cell[x*3+cellSize.width*3*y+2]);
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