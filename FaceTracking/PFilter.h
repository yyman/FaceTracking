#ifndef PFILTER_H_
#define PFILTER_H_

//'fopen' �̐錾���m�F���Ă��������B���p
#pragma warning( disable : 4996 )

#include <vector>
#include "Particle.h"

#include <stdio.h>
#include <opencv\cv.h>
#include <opencv2\opencv.hpp>
#include <opencv2\calib3d\calib3d.hpp>

#ifdef _DEBUG
//Debug���[�h�̏ꍇ
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
//Release���[�h�̏ꍇ
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

  // �e�ϐ��̏���A����
  LIMIT upper, lower;

  // �m�C�Y
  NOISE noise;

  //�p�[�e�B�N����
  int num;
    
 public:
  //�R���X�g���N�^
  PFilter();
  PFilter(int num, LIMIT upper,  LIMIT lower, NOISE noise);

  //�f�X�g���N�^
  ~PFilter();

  //�p�[�e�B�N���z��
  vector<Particle*> particles;
  // �p�[�e�B�N���z��i���O����p�j
  vector<Particle*> pre_particles;

  //��ԑJ�ڃ��f����K�p
  void predict();

  //�d�݂Ɋ�Â��T���v�����O
  void resample();

  //�d�ݕt��
  void weight(IplImage* img);

  //�p�[�e�B�N���Q�̏d�݂����ς��o��
  void  measure(Particle* r);

};
#endif /*PFILTER_H_*/