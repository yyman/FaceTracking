#ifndef OCVFD_H_
#define OCVFD_H_

#include <vector>
#include "Define.h"

class OcvFD
{
private:
	int x,y,vx,vy;
	double weight;
	Mat image;
	Rect selection;
	Rect trackWindow;
	int hsize;
	int matchesNum;
	Mat frame, hsv, hue, hist, mask, backproj;

public:
	OcvFD();
	~OcvFD();

	int get_x();
	int get_y();
	int get_vx();
	int get_vy();
	double getWeight();
	void matching(
		Mat img1,			// 画像１
		Mat img2,			// 画像２
		const std::string& featureDetectorName,		// detectorType
		const std::string& descriptorExtractorName,	// descriptorExtractorType
		const std::string& descriptorMatcherName,	// descriptorMatcherType
		bool crossCheck=true);				// マッチング結果をクロスチェックするかどうか
	
};

#endif /*OCVFD_H_*/