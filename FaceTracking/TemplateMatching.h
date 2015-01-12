#ifndef TMACHING_H_
#define TMACHING_H_

#include <vector>
#include "Define.h"
#include "FaceDetect.h"
#include "calcHSVHist.h"

class TemplateMatching
{
private:
	int tempType;
	FD fd;
	calcHSVHist ch;
	string filename,templateURL;
	
	Mat templates[181];//îzóÒÇ180Ç‹Ç≈éùÇ¬ÇΩÇﬂ
public:
	TemplateMatching();
	~TemplateMatching();

	void match(VideoCapture frame, Mat tmp_img);
	void tempChange();
	void calcRect();
	Vec2i calcVec();
	void tempRotate(uchar lr);
	bool tempPointSaveForCSV(Point pt1, Point pt2);
	void matchCSV(Mat src, string csv_path);
	void importCSV(string csv_path);
	Mat matching(Mat src, Mat temp, int flg);
};

#endif /*TMACHING_H_*/