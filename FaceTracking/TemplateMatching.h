#ifndef TMACHING_H_
#define TMACHING_H_

#include <vector>
#include "Define.h"

class TemplateMatching
{
private:
	int tempType;
public:
	TemplateMatching();
	~TemplateMatching();

	void match(VideoCapture frame, Mat tmp_img);
	void tempChange();
	void calcRect();
	Vec2i calcVec();
};

#endif /*TMACHING_H_*/