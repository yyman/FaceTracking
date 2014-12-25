#include "TemplateMatching.h"

int drag = 0;
int select_flg = 0;
Mat templateViewImg, myTemplate, roiImg;
Point point1,point2;
Rect tempRect[2];

void onMouse2 (int event, int x, int y, int flags, void *param = NULL)
{  

	if (event == CV_EVENT_LBUTTONDOWN && !drag)
	{
		/// left button clicked. ROI selection begins
		point1 = Point(x, y);
		drag = 1;
	}

	if (event == CV_EVENT_MOUSEMOVE && drag)
	{
		/// mouse dragged. ROI being selected
		Mat img1 = templateViewImg.clone();
		point2 = Point(x, y);
		rectangle(img1, point1, point2, CV_RGB(255, 0, 0), 3, 8, 0);
		imshow("templateViewImg", img1);
	}

	if (event == CV_EVENT_LBUTTONUP && drag)
	{
		point2 = Point(x, y);
		tempRect[0] = Rect(point1.x, point1.y, (x - point1.x == 0)?1:x - point1.x, (y - point1.y == 0)?1:y - point1.y);
		drag = 0;
		roiImg = templateViewImg(tempRect[0]);
		roiImg.copyTo(myTemplate);
	}
}

TemplateMatching::TemplateMatching(){
	namedWindow("templateViewImg", CV_WINDOW_AUTOSIZE);
	setMouseCallback("templateViewImg", onMouse2, "templateViewImg");
}

TemplateMatching::~TemplateMatching(void){
}

void TemplateMatching::match( VideoCapture frame, Mat tmp_img){

	bool caploop = true;
	Mat src, searchImg,showImg, gImg;
	unsigned char key = 0;
	
	// テンプレート画像
	//cv::Mat tmp_img = cv::imread("result\\model\\90.jpg", 1);
	imshow("templateViewImg", tmp_img);
	myTemplate = tmp_img.clone();
	//templateViewImgの初期化
	templateViewImg = tmp_img.clone();
	tempType = 1;

	while(caploop){
		frame >> src;
		
		cv::Mat result_img;
		if(tempType == 1){
			//グレースケール
			cvtColor(src, gImg, CV_RGB2GRAY);
			cvtColor(gImg, searchImg, CV_GRAY2RGB);
			// テンプレートマッチング
			//cv::matchTemplate(searchImg, tmp_img, result_img, CV_TM_CCOEFF_NORMED);
			cv::matchTemplate(searchImg, myTemplate, result_img, CV_TM_CCOEFF_NORMED);
		}else if(tempType == 0){
			//グレースケール
			cvtColor(src, gImg, CV_RGB2GRAY);
			cvtColor(gImg, searchImg, CV_GRAY2RGB);
			cv::matchTemplate(searchImg, myTemplate, result_img, CV_TM_CCOEFF_NORMED);
		}else if(tempType == 2){
			//二値化
			cvtColor(src, gImg, CV_RGB2GRAY);
			cv::threshold(gImg, searchImg, 85, 255, CV_THRESH_BINARY);
			cvtColor(searchImg, searchImg, CV_GRAY2RGB);
			// テンプレートマッチング
			//cv::matchTemplate(searchImg, tmp_img, result_img, CV_TM_CCOEFF_NORMED);
			cv::matchTemplate(searchImg, myTemplate, result_img, CV_TM_CCOEFF_NORMED);
		}

		// 最大のスコアの場所を探す
		cv::Rect roi_rect(0, 0, myTemplate.cols, myTemplate.rows);
		cv::Point max_pt;
		double maxVal;
		cv::minMaxLoc(result_img, NULL, &maxVal, NULL, &max_pt);
		roi_rect.x = max_pt.x;
		roi_rect.y = max_pt.y;
		std::cout << "(" << max_pt.x << ", " << max_pt.y << "), score=" << maxVal << std::endl;
		// 探索結果の場所に矩形を描画
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
		case 32:
			tempChange();
			break;
		default:
			//cout<<(char)key<<endl;
			break;
		}
	}
}

void TemplateMatching::tempChange(){
	if(tempType == 1){
		templateViewImg = imread("result\\model\\test\\resizeImg90.jpg");
		myTemplate = templateViewImg.clone();
		imshow("templateViewImg", templateViewImg);
		tempType = 0;
	}else if(tempType == 0){
		templateViewImg = imread("result\\model\\test\\binary\\resizeImg90.jpg");
		myTemplate = templateViewImg.clone();
		imshow("templateViewImg", templateViewImg);
		tempType = 2;
	}else if(tempType == 2){
		templateViewImg = imread("result\\model\\test\\binary\\resizeImg90.jpg");
		myTemplate = templateViewImg.clone();
		imshow("templateViewImg", templateViewImg);
		tempType = 1;
	}
}