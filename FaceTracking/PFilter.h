#ifndef PFILTER_H_
#define PFILTER_H_

#include <vector>
#include <opencv/cv.h>
#include "Particle.h"

using namespace std;

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