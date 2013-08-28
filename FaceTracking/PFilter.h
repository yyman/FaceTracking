#ifndef PFILTER_H_
#define PFILTER_H_

//'fopen' の宣言を確認してください。回避用
#pragma warning( disable : 4996 )

#include <vector>
#include "Particle.h"

#include <stdio.h>
#include <opencv\cv.h>
#include <opencv2\opencv.hpp>
#include <opencv2\calib3d\calib3d.hpp>

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
#endif

using namespace std;
using namespace cv;

typedef struct {
  int x,y, vx,vy;
} LIMIT;

typedef struct {
  int x,y,vx,vy;
} NOISE;


class PFilter{
 private:
  double calcLikelihood(IplImage* img, int x, int y);

  // 各変数の上限、下限
  LIMIT upper, lower;

  // ノイズ
  NOISE noise;

  //パーティクル数
  int num;
    
 public:
  //コンストラクタ
  PFilter();
  PFilter(int num, LIMIT upper,  LIMIT lower, NOISE noise);

  //デストラクタ
  ~PFilter();

  //パーティクル配列
  vector<Particle*> particles;
  // パーティクル配列（事前推定用）
  vector<Particle*> pre_particles;

  //状態遷移モデルを適用
  void predict();

  //重みに基づきサンプリング
  void resample();

  //重み付け
  void weight(IplImage* img);

  //パーティクル群の重みつき平均を出力
  void  measure(Particle* r);

};
#endif /*PFILTER_H_*/