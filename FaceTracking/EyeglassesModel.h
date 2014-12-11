#ifndef EMODEL_H_
#define EMODEL_H_

#include <vector>
#include "Define.h"

class EyeglassesModel
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
	EyeglassesModel();
	~EyeglassesModel();

	string getNameOfType(int argType);
	void printParams(const Algorithm* algo);

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

#endif /*EMODEL_H_*/