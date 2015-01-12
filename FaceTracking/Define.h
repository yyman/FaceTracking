#pragma once
#ifndef DEFINE_H_
#define DEFINE_H_


//'fopen' の宣言を確認してください。回避用
#pragma warning( disable : 4996 )

#include <vector>

#include <stdio.h>
#include <shlwapi.h>
#include <direct.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream> 
#include <opencv\cv.h>
#include <opencv2\opencv.hpp>
#include <opencv2\calib3d\calib3d.hpp>
#include <opencv\highgui.h>
#include <opencv2\highgui\highgui.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/nonfree/nonfree.hpp" // SIFT・SURFモジュール用

#include "Histogram.h"

#ifdef _DEBUG
//Debugモードの場合
#pragma comment(lib,"opencv_core245d.lib")
#pragma comment(lib,"opencv_imgproc245d.lib")
#pragma comment(lib,"opencv_highgui245d.lib")
#pragma comment(lib,"opencv_objdetect245d.lib")
#pragma comment(lib,"opencv_contrib245d.lib")
#pragma comment(lib,"opencv_features2d245d.lib")
#pragma comment(lib,"opencv_flann245d.lib")
#pragma comment(lib,"opencv_gpu245d.lib")
#pragma comment(lib,"opencv_legacy245d.lib")
#pragma comment(lib,"opencv_ts245d.lib")
#pragma comment(lib,"opencv_video245d.lib")
#pragma comment(lib,"opencv_nonfree245d.lib")
#else
//Releaseモードの場合
#pragma comment(lib,"opencv_core245.lib")
#pragma comment(lib,"opencv_imgproc245.lib")
#pragma comment(lib,"opencv_highgui245.lib")
#pragma comment(lib,"opencv_objdetect245.lib")
#pragma comment(lib,"opencv_contrib245.lib")
#pragma comment(lib,"opencv_features2d245.lib")
#pragma comment(lib,"opencv_flann245.lib")
#pragma comment(lib,"opencv_gpu245.lib")
#pragma comment(lib,"opencv_legacy245.lib")
#pragma comment(lib,"opencv_ts245.lib")
#pragma comment(lib,"opencv_video245.lib")
#pragma comment(lib,"opencv_nonfree245.lib")
#endif


#pragma comment(lib, "shlwapi.lib")

using namespace std;
using namespace cv;

#endif /*DEFINE_H_*/
