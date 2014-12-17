#ifndef TMACHING_H_
#define TMACHING_H_

#include <vector>
#include "Define.h"

class TemplateMatching
{
private:
	
public:
	TemplateMatching();
	~TemplateMatching();

	void match(VideoCapture frame, Mat tmp_img);
};

#endif /*TMACHING_H_*/