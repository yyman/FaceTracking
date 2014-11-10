#include "Hough.h"

Hough::Hough(){
}

Hough::~Hough(void){
}


void Hough::calc(Mat src){
	dst = src.clone();
	cvtColor(src, GrayImg, CV_BGR2GRAY);
	//エッジ
	Canny(GrayImg, GrayImg, 50, 200, 3);

	//Hough変換
	vector<Vec2f> lines;
	//直線の検出
	HoughLines(GrayImg, lines, 1, CV_PI/180, 200, 0, 0);

	//すべてのラインを出力する
	std::vector<Vec2f>::iterator it = lines.begin();
	for(; it!=lines.end(); ++it) 
	{
		float rho = (*it)[0], theta = (*it)[1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = saturate_cast<int>(x0 + 1000*(-b));
		pt1.y = saturate_cast<int>(y0 + 1000*(a));
		pt2.x = saturate_cast<int>(x0 - 1000*(-b));
		pt2.y = saturate_cast<int>(y0 - 1000*(a));
		line(dst, pt1, pt2, Scalar(0,255,0), 2, CV_AA);
	}
}

void Hough::show(){

	cv::namedWindow("Hough", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	cv::imshow("GrayImg", GrayImg);
	cv::imshow("Hough", dst);
}
