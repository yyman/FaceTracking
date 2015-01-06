#include "TemplateMatching.h"

int drag = 0;
int select_flg = 0;
Mat templateViewImg, myTemplate, myTemplate2, roiImg;
Point point1,point2,rect1p,rect2p;
Rect tempRect,tempRect2;
int tempNum = 1;//プラスマイナスで判断？
Point luP, rbP;//テンプレートの左上と右下はどのポイントか

int mouseType = 1;//0:ドラッグ,1:サイズ（mouseSize）固定
Size mouseSize = Size(50, 50);

void onMouse2 (int event, int x, int y, int flags, void *param = NULL)
{  
	switch (mouseType){
	case 0:
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
			tempRect = Rect(point1.x, point1.y, (x - point1.x == 0) ? 1 : x - point1.x, (y - point1.y == 0) ? 1 : y - point1.y);
			drag = 0;
			roiImg = templateViewImg(tempRect);
			roiImg.copyTo(myTemplate);
		}

		break;
	case 1://サイズ固定（cursor二つまで）
		if (event == CV_EVENT_LBUTTONDOWN && !drag)
		{
			drag = 1;//クリック判定用
		}

		if (event == CV_EVENT_MOUSEMOVE)
		{
			Mat img1 = templateViewImg.clone();
			//はみ出し防止
			if (x < mouseSize.width / 2) x = mouseSize.width / 2;
			if (x > templateViewImg.cols - mouseSize.width / 2) x = templateViewImg.cols - mouseSize.width / 2;
			if (y < mouseSize.height / 2) y = mouseSize.height / 2;
			if (y > templateViewImg.rows - mouseSize.height / 2) y = templateViewImg.rows - mouseSize.height / 2;

			point1 = Point(x - mouseSize.width / 2, y - mouseSize.height / 2);
			point2 = Point(x + mouseSize.width / 2, y + mouseSize.height / 2);
			if (tempNum > 0){
				rectangle(img1, point1, point2, CV_RGB(255, 0, 0), 3, 8, 0);
			}
			else if (tempNum < 0){
				rectangle(img1, tempRect, CV_RGB(255, 0, 0), 3, 8, 0);
				rectangle(img1, point1, point2, CV_RGB(0, 255, 0), 3, 8, 0);
			}
			else if (tempNum == 0){
				rectangle(img1, tempRect, CV_RGB(255, 0, 0), 3, 8, 0);
				rectangle(img1, tempRect2, CV_RGB(0, 255, 0), 3, 8, 0);
			}
			imshow("templateViewImg", img1);
		}

		if (event == CV_EVENT_LBUTTONUP && drag)
		{
			//テンプレート確定
			if (tempNum > 0){
				tempRect = Rect(point1.x, point1.y, mouseSize.width, mouseSize.height);
				rect1p = Point(point1.x - mouseSize.width / 2, point1.y - mouseSize.height / 2);
				roiImg = templateViewImg(tempRect);
				roiImg.copyTo(myTemplate);
				tempNum = -1;
			}
			else if (tempNum < 0){
				tempRect2 = Rect(point1.x, point1.y, mouseSize.width, mouseSize.height);
				rect2p = Point(point1.x - mouseSize.width / 2, point1.y - mouseSize.height / 2);
				roiImg = templateViewImg(tempRect2);
				roiImg.copyTo(myTemplate2);
				tempNum = 0;
			}
			drag = 0;
		}

		break;
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
	Mat src, searchImg, showImg, gImg, searchImg2, showImg2, gImg2;
	unsigned char key = 0;
	
	// テンプレート画像
	//cv::Mat tmp_img = cv::imread("result\\model\\90.jpg", 1);
	imshow("templateViewImg", tmp_img);
	//テンプレート初期化
	myTemplate = tmp_img.clone();
	myTemplate2 = tmp_img.clone();
	//templateViewImgの初期化
	templateViewImg = tmp_img.clone();
	tempType = 1;

	while(caploop){
		frame >> src;

		//////////////////////////////////////テンプレート１
		cv::Mat result_img;
		if (tempType == 1){
			//グレースケール
			cvtColor(src, gImg, CV_RGB2GRAY);
			cvtColor(gImg, searchImg, CV_GRAY2RGB);
			// テンプレートマッチング
			//cv::matchTemplate(searchImg, tmp_img, result_img, CV_TM_CCOEFF_NORMED);
			cv::matchTemplate(searchImg, myTemplate, result_img, CV_TM_CCOEFF_NORMED);
		}
		else if (tempType == 0){
			//グレースケール
			cvtColor(src, gImg, CV_RGB2GRAY);
			cvtColor(gImg, searchImg, CV_GRAY2RGB);
			cv::matchTemplate(searchImg, myTemplate, result_img, CV_TM_CCOEFF_NORMED);
		}
		else if (tempType == 2){
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
#ifdef _DEBUG
		std::cout << "(" << max_pt.x << ", " << max_pt.y << "), score=" << maxVal << std::endl;
#endif
		//////////////////////////////////////テンプレート２
		cv::Mat result_img2;
		if (tempType == 1){
			//グレースケール
			cvtColor(src, gImg2, CV_RGB2GRAY);
			cvtColor(gImg2, searchImg2, CV_GRAY2RGB);
			// テンプレートマッチング
			cv::matchTemplate(searchImg2, myTemplate2, result_img2, CV_TM_CCOEFF_NORMED);
		}
		else if (tempType == 0){
			//グレースケール
			cvtColor(src, gImg2, CV_RGB2GRAY);
			cvtColor(gImg2, searchImg2, CV_GRAY2RGB);
			cv::matchTemplate(searchImg2, myTemplate2, result_img2, CV_TM_CCOEFF_NORMED);
		}
		else if (tempType == 2){
			//二値化
			cvtColor(src, gImg2, CV_RGB2GRAY);
			cv::threshold(gImg2, searchImg2, 85, 255, CV_THRESH_BINARY);
			cvtColor(searchImg2, searchImg2, CV_GRAY2RGB);
			// テンプレートマッチング
			cv::matchTemplate(searchImg2, myTemplate2, result_img2, CV_TM_CCOEFF_NORMED);
		}

		// 最大のスコアの場所を探す
		cv::Rect roi_rect2(0, 0, myTemplate2.cols, myTemplate2.rows);
		cv::Point max_pt2;
		double maxVal2;
		cv::minMaxLoc(result_img2, NULL, &maxVal2, NULL, &max_pt2);
		roi_rect2.x = max_pt2.x;
		roi_rect2.y = max_pt2.y;
#ifdef _DEBUG
		std::cout << "(" << max_pt2.x << ", " << max_pt2.y << "), score=" << maxVal2 << std::endl;
#endif


		// 探索結果の場所に矩形を描画
		cv::rectangle(searchImg, roi_rect, cv::Scalar(0, 0, 255), 3);
		cv::rectangle(searchImg, roi_rect2, cv::Scalar(0, 255, 0), 3);


		cv::namedWindow("search image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
		cv::namedWindow("result image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
		cv::namedWindow("result image2", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);

		cv::imshow("search image", searchImg);
		cv::imshow("result image", result_img);
		cv::imshow("result image2", result_img2);


		/////////////////二つのテンプレで最大のところを探す
		Mat sum_result_img;
	    Point maxp;
		double max1 = 0;
		if (mouseType == 1 && tempNum == 0){
			calcRect();
			Rect r = Rect(luP,rbP);
			sum_result_img = Mat(cv::Size(result_img.cols - r.width, result_img.rows - r.height), CV_64F, cv::Scalar::all(0));
			//cv::imshow("sum result image", sum_result_img);

			//どっちのテンプレートが下かでズレが変わる
			int yinit = 0;
			int ybottom = result_img.rows - r.height -1;
			if (rect1p.y > rect2p.y){
				yinit = r.height;
				ybottom = result_img.rows;
			}

			for (int y = yinit; y < ybottom; y++)
			{
				for (int x = 0; x < result_img.cols - r.width; x++)
				{
					double p, p2;
					if (yinit > 0){
						p = result_img.at<double>(y - r.height, x);
						p2 = result_img2.at<double>(y, x);
					}
					else{
						p = result_img.at<double>(y, x);
						p2 = result_img2.at<double>(y + r.height, x);
					}
					
					double ps;

					ps = (p + p2) / 2;
					sum_result_img.at<double>(y, x) = ps;//はみ出すからfor文見直さないと

					//cout << p << "," << p2 << "," << ps << endl;

					//waitKey(0);
					if (max1 < ps)
					{
						max1 = ps;
						maxp = Point(x, y);
						if (ps == 255)
						{
							//waitKey(0);
							cout << x << "," << y << ":" << ps << ":" << max1 << endl;
						}
					}
				}
				cout << y << ":" << r.height << ":" << result_img.rows << ":" << max1 << endl;
			}
			cv::imshow("sum result image", sum_result_img);
			cout << maxp.x << "," << maxp.y << ":" << max1 << endl;
			r.x = maxp.x;
			r.y = maxp.y;
			cv::rectangle(searchImg, r, cv::Scalar(255, 0, 0), 3);
		}

		key = waitKey(5);

		switch(key){
		case 27: 
			caploop = false;
			break;
		case 32:
			tempChange();
			break;
		case 'c':
			mouseType = 1;
			break;
		case 'd':
			mouseType = 0;
			break;
		case 'r':
			tempNum = 1;
			break;
		default:
			//cout<<(char)key<<endl;
			break;
		}
	}
}

void TemplateMatching::tempChange(){
	if (tempType == 1){
		templateViewImg = imread("result\\model\\test\\resizeImg90.jpg");
		myTemplate = templateViewImg.clone();
		imshow("templateViewImg", templateViewImg);
		tempType = 0;
	}
	else if (tempType == 0){
		templateViewImg = imread("result\\model\\test\\binary\\resizeImg90.jpg");
		myTemplate = templateViewImg.clone();
		imshow("templateViewImg", templateViewImg);
		tempType = 2;
	}
	else if (tempType == 2){
		templateViewImg = imread("result\\model\\test\\binary\\resizeImg90.jpg");
		myTemplate = templateViewImg.clone();
		imshow("templateViewImg", templateViewImg);
		tempType = 1;
	}
}

void TemplateMatching::calcRect(){
	if (rect1p.x <= rect2p.x){
		if (rect1p.y <= rect2p.y){
			luP = rect1p;
			rbP = rect2p;
		}
		else{
			luP = Point(rect1p.x, rect2p.y);
			rbP = Point(rect2p.x, rect1p.y);
		}
	}
	else{
		if (rect1p.y > rect2p.y){
			luP = rect2p;
			rbP = rect1p;
		}
		else{
			luP = Point(rect2p.x, rect1p.y);
			rbP = Point(rect1p.x, rect2p.y);
		}
	}
}