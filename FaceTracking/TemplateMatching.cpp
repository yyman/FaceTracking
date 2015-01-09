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

Mat glasses[181];
//テンプレート初期角度
int temp_i = 90;

bool saveFlg = false;

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

	bool faceDetected = false;
	Size faceSize;
	Mat faceImage;
	Rect faceRect;

	while(caploop){
		frame >> src;

		//顔検出
		faceDetected = fd.detect(src,ch);
		if(faceDetected){
			faceRect = fd.getFaceImage().rect;

			//faceRectを拡大
			faceRect.x = (faceRect.x >= mouseSize.width) ? faceRect.x - mouseSize.width : 0;
			faceRect.y = (faceRect.y >= mouseSize.height) ? faceRect.y - mouseSize.height : 0;
			faceRect.width = (faceRect.x + faceRect.width + mouseSize.width * 2 < src.cols) ? faceRect.width + mouseSize.width * 2 : src.cols - faceRect.x;
			faceRect.height = (faceRect.y + faceRect.height + mouseSize.height * 2 < src.rows) ? faceRect.height + mouseSize.height * 2 : src.rows - faceRect.y;
			faceSize = faceImage.size();

			//顔検出矩形からマウスサイズ拡大したROIを取り出す
			faceImage = src(faceRect);

			src = faceImage.clone();

			faceDetected = false;
		}

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


		cv::namedWindow("result image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
		cv::namedWindow("result image2", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);

		cv::imshow("result image", result_img);
		cv::imshow("result image2", result_img2);


		/////////////////二つのテンプレで最大のところを探す
		Mat sum_result_img;
		Point maxp;
		double max1 = 0;
		Vec2d vec = calcVec();
		if (mouseType == 1 && tempNum == 0 && vec[0] != 0 && vec[1] != 0){
			//まだセーブしてなかったら
			if (!saveFlg)
			{
				saveFlg = tempPointSaveForCSV(rect1p, rect2p);
			}
			//calcRect();
			//Rect r = Rect(luP,rbP);
			int vx = vec[0];
			int vy = vec[1];
			sum_result_img = Mat(cv::Size(result_img.cols - abs(vx), result_img.rows - abs(vy)), CV_32F, cv::Scalar::all(0));
			//cv::imshow("sum result image", sum_result_img);

			for (int y = 0; y < result_img.rows - abs(vy) - 1; ++y)
			{
				for (int x = 0; x < result_img.cols - abs(vx); ++x)
				{
					float p, p2;
					if (vy > 0)
					{
						if (vx > 0)
						{
							if (result_img.data == NULL){
								cout << " null yade" << endl;
							}
							//cout << x << "," << y << ":" << vx << "," << vy << endl;
							p = result_img.at<float>(y, x);
							p2 = result_img2.at<float>(y + vy, x + vx);
						}
						else
						{
							p = result_img.at<float>(y, x - vx);
							p2 = result_img2.at<float>(y + vy, x);
						}
					}
					else
					{
						if (vx > 0)
						{
							p = result_img.at<float>(y - vy, x);
							p2 = result_img2.at<float>(y, x + vx);
						}
						else
						{
							p = result_img.at<float>(y - vy, x - vx);
							p2 = result_img2.at<float>(y, x);
						}
					}

					float ps;

					ps = p + p2;
					sum_result_img.at<float>(y, x) = ps;//はみ出すからfor文見直さないと

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
				//cout << y << ":" << result_img.rows << ":" << max1 << endl;
			}
			cv::imshow("sum result image", sum_result_img);
			int mpx, mpy;
			if (vx > 0)
			{
				if ( vy > 0)
				{
					mpx = maxp.x;
					mpy = maxp.y;
				}
				else
				{
					mpx = maxp.x;
					mpy = maxp.y - vy;
				}
			}
			if (vx < 0)
			{
				if ( vy > 0)
				{
					mpx = maxp.x - vx;
					mpy = maxp.y;
				}
				else
				{
					mpx = maxp.x - vx;
					mpy = maxp.y - vy;
				}
			}
			Point mp(mpx, mpy);
			cout << maxp.x << "," << maxp.y << ":" << mp.x << "," << mp.y << "), score=" << max1 << endl;
			Rect r(mp.x, mp.y, vx + mouseSize.width, vy + mouseSize.height);
			cv::rectangle(searchImg, r, cv::Scalar(255, 0, 0), 3);
		}

		cv::namedWindow("search image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
		cv::imshow("search image", searchImg);

		key = waitKey(5);

		switch(key){
		case 27: 
			caploop = false;
			break;
		case 32:
			tempChange();
			tempNum = 1;
			break;
		case 'c':
			mouseType = 1;
			break;
		case 'm':
			mouseType = 0;
			break;
		case 'r':
			tempNum = 1;
			break;
		case 'd'://テンプレートの右回転
			tempRotate('R');
			tempNum = 1;
			break;
		case 'a'://テンプレートの左回転
			tempRotate('L');
			tempNum = 1;
			break;
		default:
			//cout<<(char)key<<endl;
			if (tempType == 1) tempChange(); 
			break;
		}
	}
}

void TemplateMatching::tempChange(){
	if (tempType == 1){
		templateURL = "result\\model\\test\\resizeImg90.jpg";
		templateViewImg = imread(templateURL);
		myTemplate = templateViewImg.clone();
		imshow("templateViewImg", templateViewImg);
		tempType = 0;
	}
	else if (tempType == 0){
		templateURL = "result\\model\\test\\binary\\resizeImg90.jpg";
		templateViewImg = imread(templateURL);
		myTemplate = templateViewImg.clone();
		imshow("templateViewImg", templateViewImg);
		tempType = 2;
	}
	else if (tempType == 2){
		templateURL = "result\\model\\test\\binary\\resizeImg90.jpg";
		templateViewImg = imread(templateURL);
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

Vec2i TemplateMatching::calcVec(){
	int x = rect2p.x - rect1p.x;
	int y = rect2p.y - rect1p.y;
	Vec2i v(x,y);
	cout << v[0] << "," << v[1] << endl;
	return v;
}

void TemplateMatching::tempRotate(uchar LR){
	bool showLoop = true;
	int i;
	if(LR == 'L')
	{
		i = (temp_i != 180) ? temp_i + 3 : 0;
	}
	else
	{
		i = (temp_i != 0) ? temp_i - 3 : 0;
	}
	temp_i = i;
	ostringstream oss;
	oss << i << ".jpg";
	templateURL = "result\\model\\test\\resizeImg" + oss.str();
	templateViewImg = imread(templateURL);
	myTemplate = templateViewImg.clone();
	imshow("templateViewImg", templateViewImg);
}

bool TemplateMatching::tempPointSaveForCSV(Point pt1, Point pt2){
	bool flg = false;
	filename = "result\\model\\test\\data.csv";
	ofstream ofs(filename, ios::app);
	if ( ofs )
	{
		// current date/time based on current system
		time_t now = time(0);

		// convert now to string form
		char* dt = ctime(&now);

		ofs << temp_i << "," << templateURL << "," << pt1.x << "," << pt1.y << "," << pt2.x << "," << pt2.y << "," << dt << endl;
		flg = true;
	}
	return flg;
}