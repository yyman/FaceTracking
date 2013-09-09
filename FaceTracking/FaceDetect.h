#ifndef FACE_DETECT_H_
#define FACE_DETECT_H_

#include <iostream>
#include "PFilter.h"
#include "calcHSVHist.h"


class FD
{
private:
    char name[40];
    int age;
	Mat grayImage;
public:
    FD();    //コンストラクタ
    void setname(char* ss) { strcpy(name,ss); }
    void setage(int tosi) { age=(tosi<0) ? 0: tosi; }
    char* getname() { return name; }
    int getage() { return age; }
    void detect(Mat src, calcHSVHist ch);
    ~FD();    //デストラクタ
};


#endif /*FACE_DETECT_H_*/