#ifndef FACE_DETECT_H_
#define FACE_DETECT_H_

#include <iostream>
#include "PFilter.h"



typedef struct {
	Mat img;
	Rect rect;
} FaceImage;

class FD
{
private:
    char name[40];
    int age;
	Mat grayImage;
	FaceImage faceImage;
public:
    FD();    //コンストラクタ
    void setname(char* ss) { strcpy(name,ss); }
    void setage(int tosi) { age=(tosi<0) ? 0: tosi; }
    char* getname() { return name; }
    int getage() { return age; }
    bool detect(Mat src, calcHSVHist ch);
	FaceImage getFaceImage();
    ~FD();    //デストラクタ
};


#endif /*FACE_DETECT_H_*/