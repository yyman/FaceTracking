#include "Block.h"


Block::Block(){
}

Block::Block(Mat _src, Size _blockSize, Size _cellSize)
{
	src = _src;
	//cvtColor(_src,src,CV_BGR2HSV);
	blockSize = _blockSize;
	cellSize = _cellSize;//奇数のみ

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
	
	//cout<<cellW<<","<<cellH<<endl;
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

Block::~Block(){
}

//srcの取得
Mat Block::getSrc(){
	return src;
}

//セルの幅の取得
int Block::getCellW(){
	return cellW;
}

//セルの高さの取得
int Block::getCellH(){
	return cellH;
}

//ブロックサイズの取得
Size Block::getBlockSize(){
	return blockSize;
}

//セルサイズの取得
Size Block::getCellSize(){
	return cellSize;
}

//セルの取得
vector<double> Block::getCell(){
	return cell;
}

//各セルの色を取得（0始まり）
cv::Scalar Block::getColor(int x,int y){
	cv::Scalar color = cv::Scalar(cell[x*3+cellSize.width*3*y+0],
							cell[x*3+cellSize.width*3*y+1],
							cell[x*3+cellSize.width*3*y+2]);
	return color;
}

//各セルの色を取得（0始まり）
cv::Scalar Block::getCenterColor(){
	return centerColor;
}


void Block::calcAverageColor(){
	for(int y=0;y<cellSize.height;y++){
		for(int x=0;x<cellSize.width;x++){
			cell[x*3+cellSize.width*3*y+0] = cell[x*3+cellSize.width*3*y+0]/(cellW*cellH); //B
			cell[x*3+cellSize.width*3*y+1] = cell[x*3+cellSize.width*3*y+1]/(cellW*cellH); //G
			cell[x*3+cellSize.width*3*y+2] = cell[x*3+cellSize.width*3*y+2]/(cellW*cellH); //R
		}
	}
}

Mat Block::getAverageImg(){
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

