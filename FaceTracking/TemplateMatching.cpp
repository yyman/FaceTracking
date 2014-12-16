#include "TemplateMatching.h"

TemplateMatching::TemplateMatching(){
}

TemplateMatching::~TemplateMatching(void){
}

void TemplateMatching::match( VideoCapture frame){

	bool caploop = true;
	Mat searchImg,showImg;
	unsigned char key = 0;

	while(caploop){
		frame >> searchImg;

		// �e���v���[�g�摜
		cv::Mat tmp_img = cv::imread("result\\model\\90.jpg", 1);
		imshow("temp image", tmp_img);

		cv::Mat result_img;
		// �e���v���[�g�}�b�`���O
		cv::matchTemplate(searchImg, tmp_img, result_img, CV_TM_CCOEFF_NORMED);

		// �ő�̃X�R�A�̏ꏊ��T��
		cv::Rect roi_rect(0, 0, tmp_img.cols, tmp_img.rows);
		cv::Point max_pt;
		double maxVal;
		cv::minMaxLoc(result_img, NULL, &maxVal, NULL, &max_pt);
		roi_rect.x = max_pt.x;
		roi_rect.y = max_pt.y;
		std::cout << "(" << max_pt.x << ", " << max_pt.y << "), score=" << maxVal << std::endl;
		// �T�����ʂ̏ꏊ�ɋ�`��`��
		cv::rectangle(searchImg, roi_rect, cv::Scalar(0,0,255), 3);

		cv::namedWindow("search image", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
		cv::namedWindow("result image", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
		cv::imshow("search image", searchImg);
		cv::imshow("result image", result_img);

		key = waitKey(5);

		switch(key){
		case 27: 
			caploop = false;
			break;
		default:
			//cout<<(char)key<<endl;
			break;
		}
	}

}