#include "MShift.h"

const float hranges[] = {0, 180};
const float* phranges;

MShift::MShift(){
	x = 0;
	y = 0;
	vx = 0;
	vy = 0;
	weight = 0;
	trackWindow;
	hsize = 16;
	phranges = hranges;
	matchesNum = 0;
}

MShift::~MShift(void){
	image.release();
}

Mat MShift::run(Mat src, Rect rect){
	src.copyTo(image);
	cvtColor(image, hsv, CV_BGR2HSV);
	selection = rect;
	inRange(hsv, Scalar(0, 69, 53),
		Scalar(180, 256, 256), mask);
	int ch[] = {0, 0};
	hue.create(hsv.size(), hsv.depth());
	mixChannels(&hsv, 1, &hue, 1, ch, 1);

	Mat roi(hue, selection), maskroi(mask, selection);
	calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
	normalize(hist, hist, 0, 255, CV_MINMAX);
	
	trackWindow = selection;

	calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
	backproj &= mask;
	imshow("backproj", backproj);
	RotatedRect trackBox = CamShift(backproj, trackWindow,TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ));
	if( trackWindow.area() <= 1 )
	{
		int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
		trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,
			trackWindow.x + r, trackWindow.y + r) &
			Rect(0, 0, cols, rows);
	}

	ellipse( image, trackBox, Scalar(0,0,255), 3, CV_AA );

	return image;
}