#ifndef FACE_DETECT_H_
#define FACE_DETECT_H_

#include <iostream>
#include "PFilter.h"


class FD
{
private:
    char name[40];
    int age;
	Mat grayImage;
	Mat faceImage;
public:
    FD();    //�R���X�g���N�^
    void setname(char* ss) { strcpy(name,ss); }
    void setage(int tosi) { age=(tosi<0) ? 0: tosi; }
    char* getname() { return name; }
    int getage() { return age; }
    bool detect(Mat src, calcHSVHist ch);
	Mat getFaceImage();
    ~FD();    //�f�X�g���N�^
};


#endif /*FACE_DETECT_H_*/