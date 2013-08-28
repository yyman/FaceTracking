
#include <iostream>
#include "PFilter.h"
#include "FaceDetect.h"


string cascadeName = "data\\haarcascade_frontalface_alt.xml";//�w�K�ς݌��o��
CascadeClassifier cascade;
Mat faceImage;

FD::FD()    //�R���X�g���N�^�̒�`
{
	//�����ǂݍ���
	// �w�K�ς݌��o��̓ǂݍ���
	cout<<"���o��ǂݍ��݊J�n"<<endl;
    if(!cascade.load(cascadeName)){
		cout<<"���o�킪�ǂݍ��߂܂���ł���"<<endl;
	}
    cout << "�R���X�g���N�^�Ăяo���_n";
}

FD::~FD()    //�f�X�g���N�^�̒�`
{
	grayImage.release();
    cout << "FD�f�X�g���N�^�Ăяo���_n";
}

void FD::detect(Mat src)
{
	//�猟�o

    cvtColor(src, grayImage, CV_RGB2GRAY);
	// �猟�o�����s
    vector<Rect> faces;
    cascade.detectMultiScale(
        grayImage,   // �摜
        faces,       // �o�͂�����`
        1.1,         // �k���p�̃X�P�[��
        2,           // �ŏ��̓��[��
        CV_HAAR_SCALE_IMAGE,  // �t���O
		cv::Size(30, 30) // �ŏ��̋�`
    );
    // ��`��`��
    for (vector<Rect>::iterator iter = faces.begin(); iter != faces.end(); iter ++) {
		faceImage = src(*iter);
		imshow("face", faceImage);
	}
}
