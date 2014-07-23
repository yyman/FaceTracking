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
	src.release();
	averageSrc.release();
}

double CalcLike::calcLikelihood(Block bl){
	double result = 0.0;
	double dist = 0.0, sigma = 50.0, sumDist = 0.0; 
	int cellCnt = base.getCellSize().height * base.getCellSize().width;//cellの数

	// RGB 色空間の場合
	unsigned char b, g, r;
	double baseB, baseG, baseR;
	Scalar blColor,baseColor;
	for(int y = 0; y < base.getCellSize().height; y++){
		for(int x = 0; x < base.getCellSize().width; x++){
			blColor = bl.getColor(x,y);
			b = blColor[0];  // B
			g = blColor[1];  // G
			r = blColor[2];  // R

			baseColor = base.getColor(x,y);
			baseB = baseColor[0];
			baseG = baseColor[1];
			baseR = baseColor[2];
			//cout<<clb<<","<<clg<<","<<clr<<endl;
			// baseの中心色らしさをユークリッド距離として求める

			switch (y)
			{
			case 0:
			case 4:
				sumDist += sqrt((baseB-b)*(baseB-b) + (baseG-g)*(baseG-g) + (baseR-r)*(baseR-r));//最高で約444（sqrt(256*3)）離れる？
				break;
			default:
				sumDist += sqrt((baseB-b)*(baseB-b) + (baseG-g)*(baseG-g) + (baseR-r)*(baseR-r));
				break;
			}
			//cout<<dist<<endl;
			//imshow("blave",bl.getAverageImg());
		}
	}
	dist = sumDist / cellCnt;//距離の加重平均
	cout << dist << endl; 
	// 距離(dist)を平均、sigmaを分散として持つ、正規分布を尤度関数とする
	result = 1.0 / (sqrt(2.0*CV_PI)*sigma) * expf(-dist*dist/(2.0*sigma*sigma));
	return result;
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
	//cout<<cnt<<endl;

	return averageSrc;
}

Size CalcLike::getCellSize(){
	return cellSize;
}