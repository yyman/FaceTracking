#ifndef TMACHING_H_
#define TMACHING_H_

#include <vector>
#include "Define.h"
#include "FaceDetect.h"
#include "calcHSVHist.h"


typedef struct {
	int index;
	string path;
	Point p1,p2;
} TempModel;

class TemplateMatching
{
private:
	int tempType;
	FD fd;
	calcHSVHist ch;
	string filename,templateURL;
	
	Mat templates[181];//配列を180まで持つため
	bool tmFlg;
	TempModel tm[181];
	
	//初期角度を90でテンプレート取得
	int init_a;
	int diff_a;
public:
	TemplateMatching();
	~TemplateMatching();

	void match(VideoCapture frame, Mat tmp_img);
	void tempChange();
	Rect calcRect(Mat result_img, Mat temp);
	Vec2i calcVec(Point r1p, Point r2p);
	void tempRotate(uchar lr);
	bool tempPointSaveForCSV(Point pt1, Point pt2);
	void matchCSV(Mat src_img, string csv_path);
	void importCSV(string csv_path);
	Mat matching(Mat src, Mat temp, int flg);
	Mat sumMatchingResult(Mat result_img1, Mat result_img2, Vec2i v);
	Rect maxRectSumResult(Mat sum_result, Vec2i vec, double &maxVal);
	Rect maxRectResult(Mat result_img, double &max_val);
};

#endif /*TMACHING_H_*/