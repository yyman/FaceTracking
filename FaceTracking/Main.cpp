
#include <iostream>
#include "PFilter.h"
#include "FaceDetect.h"
#include "calcHSVHist.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;

int main(int argc, char** argv)
{
  const int w = 640;//��ʂ̕�
  const int h = 480;//��ʂ̍���
  const int num = 300;//�p�[�e�B�N���̐�

 //��ԕϐ��̏���A����
  LIMIT upper, lower;
  upper.x=w; upper.y= h; upper.vx=w/2; upper.vy=h/2;
  lower.x=0; lower.y=0; lower.vx=-w/2; lower.vy=-h/2;
  
  // �m�C�Y�̍ő�l
  NOISE noise;
  noise.x=50; noise.y=50; noise.vx=100; noise.vy=100;

  // �p�[�e�B�N���t�B���^
  PFilter* pf = new PFilter(num, upper, lower, noise);
  // �ʒu����̏o�͗p
  Particle *p = new Particle();


  // OpenCV���g����USB�J��������L���v�`������
  CvCapture *capture = 0;
  capture = cvCreateCameraCapture(0);
  cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, w);
  cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, h);

  // ���摜�ۑ��p�\����
  CvVideoWriter *vw;
  // �r�f�I���C�^�\���̂��쐬�i 640x480, 10fps, avi(mpeg4) �j
 // vw = cvCreateVideoWriter ("result/cap.avi", CV_FOURCC ('X', 'V', 'I', 'D'), 10, cvSize ((int) w, (int) h));

  // �ϑ��摜
  IplImage* img = cvCreateImage(cvSize(w,h), 8, 3);
  // ���ʉ摜
  IplImage* dst = cvCreateImage(cvSize(w,h), 8, 3);

  // �E�B���h�E�̐���
  cvNamedWindow("img");
  cvNamedWindow("dst");

  img = cvQueryFrame (capture);
 
/*#ifdef _DEBUG // 1�X�e�b�v�ڂ̎��s���ʂ�ۑ�����
  cvCopy(img, dst);
  cvSaveImage("img/000.jpg", dst);

  for(int i=0; i<num; i++){
    cvCircle(dst, cvPoint( pf->particles[i]->get_x(), pf->particles[i]->get_y() ),
	     2, CV_RGB(0,0,255), CV_FILLED);
  }
  cvSaveImage("img/000-particle.jpg", dst);

  pf->predict();
  cvCopy(img, dst);
  for(int i=0; i<num; i++){
    cvCircle(dst, cvPoint( pf->pre_particles[i]->get_x(), pf->pre_particles[i]->get_y() ),
	     2, CV_RGB(0,255,0), CV_FILLED);
  }
  cvSaveImage("img/001-predict.jpg", dst);

  pf->weight(img);
  cvCopy(img, dst);
  for(int i=0; i<num; i++){
    cvCircle(dst, cvPoint( pf->pre_particles[i]->get_x(), pf->pre_particles[i]->get_y() ), 
	     1*pf->pre_particles[i]->getWeight()*1000, CV_RGB(0, 255, 255) );
  }
  cvSaveImage("img/002-weight.jpg", dst);

  pf->measure(p);
  cvCopy(img, dst);
  cvCircle(dst, cvPoint( p->get_x(), p->get_y() ),
	   10, CV_RGB(255, 120, 120), CV_FILLED);
  cvSaveImage("img/003-measure.jpg", dst);

  pf->resample();
  cvCopy(img, dst);
  for(int i=0; i<num; i++){
    cvCircle(dst, cvPoint( pf->particles[i]->get_x(), pf->particles[i]->get_y() ), 
	     2, CV_RGB(0, 0, 255), CV_FILLED);
  }
  cvSaveImage("img/004-resample.jpg", dst);
#endif*/

  unsigned char key =0;
  bool particleFlag = true;
  bool measureFlag = true;

  //�猟�o�p�N���X
  FD fd;

  for(;;){
    img = cvQueryFrame (capture);
    cvCopy(img, dst);

    pf->predict();  

    pf->weight(img);

    pf->measure(p);

    pf->resample();

    // �p�[�e�B�N���̕\��
    if(particleFlag){
      for(int i=0; i<num; i++){
	cvCircle(dst, cvPoint( pf->particles[i]->get_x(), pf->particles[i]->get_y() ), 
		 2, CV_RGB(0, 0, 255), CV_FILLED);
      }
    }
    // ���̈ʒu�i�p�[�e�B�N���̏d�S�j���茋�ʂ̕\��
    if(measureFlag){
      cvCircle(dst, cvPoint( p->get_x(), p->get_y() ), 10, CV_RGB(255,20,20), CV_FILLED);
    }
    
    
    // �r�f�I�ɏ����o��
  //  cvWriteFrame (vw, dst);

    cvShowImage("img", img);
    cvShowImage("dst", dst);
    
	calcHSVHist ch;
	fd.detect(img,ch);

    key = cvWaitKey(50);
    if( key == 27 ){
      break;
    }
    switch( (char)key ){
    case 'p':
      particleFlag = !particleFlag;
      break;
    case 'm':
      measureFlag = !measureFlag;
      break;
    default:
      break;
    }

  }
  cout << "test end." << endl;
  return 0;
}