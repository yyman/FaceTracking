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
public:
	TemplateMatching();
	~TemplateMatching();

	void match(VideoCapture frame, Mat tmp_img);
	void tempChange();
	void calcRect();
	Vec2i calcVec();
	void tempRotate(uchar lr);
	bool tempPointSaveForCSV(Point pt1, Point pt2);
};

#endif /*TMACHING_H_*/