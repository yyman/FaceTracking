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
	const std::string& featureDetectorName;		// detectorType
	const std::string& descriptorExtractorName;	// descriptorExtractorType
	const std::string& descriptorMatcherName;	// descriptorMatcherType
	cv::Ptr<cv::FeatureDetector> detector;

public:
	OcvFD(
		const std::string& _featureDetectorName,		// detectorType
		const std::string& _descriptorExtractorName,	// descriptorExtractorType
		const std::string& _descriptorMatcherName	// descriptorMatcherType
		);
	~OcvFD();

	string OcvFD::getNameOfType(int argType);
	void OcvFD::printParams(const Algorithm* algo);

	int get_x();
	int get_y();
	int get_vx();
	int get_vy();
	double getWeight();
	void matching(
		Mat img1,			// 画像１
		Mat img2,			// 画像２
		bool crossCheck=true);				// マッチング結果をクロスチェックするかどうか
	
};

#endif /*OCVFD_H_*/