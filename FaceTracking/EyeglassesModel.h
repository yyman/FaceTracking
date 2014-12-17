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
		Mat img1,			// �摜�P
		Mat img2,			// �摜�Q
		const std::string& featureDetectorName,		// detectorType
		const std::string& descriptorExtractorName,	// descriptorExtractorType
		const std::string& descriptorMatcherName,	// descriptorMatcherType
		bool crossCheck=true);				// �}�b�`���O���ʂ��N���X�`�F�b�N���邩�ǂ���
	
};

#endif /*EMODEL_H_*/