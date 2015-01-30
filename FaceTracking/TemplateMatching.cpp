#include "TemplateMatching.h"

int drag = 0;
int select_flg = 0;
Mat templateViewImg, myTemplate, myTemplate2, roiImg;
Point point1,point2,rect1p,rect2p;
Rect tempRect,tempRect2;
int tempNum = 1;//プラスマイナスで判断？

int mouseType = 1;//0:ドラッグ,1:サイズ（mouseSize）固定
//Size mouseSize = Size(25, 25);//実写用
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
			
			cv::putText(img1, to_string(temp_i), cv::Point(50,50), cv::FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(0,0,200), 2, CV_AA);
			imshow("templateViewImg", img1);
		}

		if (event == CV_EVENT_LBUTTONUP && drag)
		{
			//テンプレート確定
			if (tempNum > 0){
				tempRect = Rect(point1.x, point1.y, mouseSize.width, mouseSize.height);
				//テンプレートの左上（基準点）を保存
				rect1p = Point(point1.x, point1.y);
				roiImg = templateViewImg(tempRect);
				roiImg.copyTo(myTemplate);
				tempNum = -1;
			}
			else if (tempNum < 0){
				tempRect2 = Rect(point1.x, point1.y, mouseSize.width, mouseSize.height);
				//テンプレートの左上（基準点）を保存
				rect2p = Point(point1.x, point1.y);
				roiImg = templateViewImg(tempRect2);
				roiImg.copyTo(myTemplate2);
				tempNum = 0;
				saveFlg = false;
			}
			drag = 0;
		}

		break;
	}
}

TemplateMatching::TemplateMatching(){
	namedWindow("templateViewImg", CV_WINDOW_AUTOSIZE);
	setMouseCallback("templateViewImg", onMouse2, "templateViewImg");

	for(int i = 0; i <= 180; i+=3)
	{	
		ostringstream oss;
		oss << i << ".jpg";
		templateURL = "result\\model\\test\\resizeImg" + oss.str();
		templates[i] = imread(templateURL);
	}

	for(int i = 45; i <= 135; i+=3)
	{	
		ostringstream oss;
		oss << i << ".jpg";
		templateURL = "result\\model\\test2c\\resizeImg" + oss.str();
		templatesR[i] = imread(templateURL);
	}

	tmFlg = false;
	
	//初期角度を90でテンプレート取得
	init_a = 90;
	diff_a = 15;
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
		//faceDetected = fd.detect(src,ch);
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
		Mat result_img;// = matching(src, myTemplate, CV_TM_CCOEFF_NORMED);
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
		Vec2d vec = calcVec(rect1p, rect2p);
		if (mouseType == 1 && tempNum == 0 && vec[0] != 0 && vec[1] != 0){
			//まだセーブしてなかったら
			if (!saveFlg)
			{
				saveFlg = tempPointSaveForCSV(rect1p, rect2p);
			}
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
					sum_result_img.at<float>(y, x) = ps;

					//cout << p << "," << p2 << "," << ps << endl;

					//waitKey(0);
					if (max1 < ps)
					{
						max1 = ps;
						maxp = Point(x, y);
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
#ifdef _DEBUG
			cout << maxp.x << "," << maxp.y << ":" << mp.x << "," << mp.y << "), score=" << max1 << endl;
#endif
			Rect r(mp.x, mp.y, vx + mouseSize.width, vy + mouseSize.height);
			cv::rectangle(searchImg, r, cv::Scalar(255, 0, 0), 3);
		}

		cv::namedWindow("search image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
		cv::imshow("search image", searchImg);

		
		//string path = "result\\model\\test\\data.csv";
		//matchCSV(searchImg, path);
		//waitKey(0);

		key = waitKey(1);

		switch(key){
		case 27: 
			caploop = false;
			destroyAllWindows();
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

Rect TemplateMatching::calcRect(Mat result_img, Mat temp){
	// 最大のスコアの場所を探す
	cv::Rect roi_rect(0, 0, temp.cols, temp.rows);
	cv::Point max_pt;
	double maxVal;
	cv::minMaxLoc(result_img, NULL, &maxVal, NULL, &max_pt);
	roi_rect.x = max_pt.x;
	roi_rect.y = max_pt.y;
#ifdef _DEBUG
	std::cout << "(" << max_pt.x << ", " << max_pt.y << "), score=" << maxVal << std::endl;
#endif

	return roi_rect;
}

Vec2i TemplateMatching::calcVec(Point r1p, Point r2p){
	int x = r2p.x - r1p.x;
	int y = r2p.y - r1p.y;
	Vec2i v(x,y);
	//cout << v[0] << "," << v[1] << endl;
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
		i = (temp_i != 0) ? temp_i - 3 : 180;
	}
	temp_i = i;
	templateViewImg = templates[i].clone();//とりあえずクローンしとく
	myTemplate = templateViewImg.clone();
	imshow("templateViewImg", templateViewImg);
}

//pt1とpt2はそのまま保存（Rect用にできるだけテンプレートの右上推奨）
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
		//templateURLは現状意味なし
		ofs << temp_i << "," << templateURL << "," << pt1.x << "," << pt1.y << "," << pt2.x << "," << pt2.y << "," << dt;
		flg = true;
	}
	return flg;
}

void TemplateMatching::matchCSV(Mat src_img, string csv_path){
	//元ソースを変更しないように
	Mat src = src_img.clone();
	Mat dst0 = src_img.clone(), dst1 = src_img.clone(), dst2 = src_img.clone();
	
	//CSVを読み込んでいなかったら読み込む
	if(!tmFlg)
	{
		importCSV(csv_path);
		tmFlg = true;
	}
	//tempType = 3;
	
	const int ai = 31;
	int64 start = cv::getTickCount();

	//各変数を３つずつの配列で作成（コンストラクタで初期角度を指定）
	//init_i - diff_i から init_i + diff_i の３パターンで計算(配列は０，１，２順)
	Rect tr1[ai], roi_rect1[ai], tr2[ai], roi_rect2[ai], sum_roi_rect[ai];
	Mat template1[ai], template2[ai], result_img1[ai], result_img2[ai], sum_result[ai]; 
	double maxVal1[ai], maxVal2[ai], sum_maxVal[ai];
	Vec2i vec[ai];
	
	//for (int a = init_a - diff_a, i = 0; a <= init_a + diff_a; a+=diff_a, i++){
	for (int a = 45, i = 0; a <= 135; a+=3, i++){
		//テンプレート1
		tr1[i] = Rect(tm[a].p1.x, tm[a].p1.y, mouseSize.width, mouseSize.height);
		template1[i] = templates[a](tr1[i]);
		result_img1[i] = matching(src, template1[i], CV_TM_CCOEFF_NORMED);
		roi_rect1[i] = maxRectResult(result_img1[i], maxVal1[i]);

		//テンプレート2
		tr2[i] = Rect(tm[a].p2.x, tm[a].p2.y, mouseSize.width, mouseSize.height);
		template2[i] = templates[a](tr2[i]);
		result_img2[i] = matching(src, template2[i], CV_TM_CCOEFF_NORMED);
		roi_rect2[i] = maxRectResult(result_img2[i], maxVal2[i]);

		//ベクトル取得
		vec[i] = calcVec(tm[a].p1, tm[a].p2);
		//ずらして共通の結果画像を求める
		sum_result[i] = sumMatchingResult(result_img1[i], result_img2[i], vec[i]);

		// 最大のスコアの場所を探す
		sum_roi_rect[i] = maxRectSumResult(sum_result[i], vec[i], sum_maxVal[i]);
	}

	//角度の変更（１．２以上だったら左右大きい方へ３度変える）
	/*if (sum_maxVal[1] < 1.6 || sum_maxVal[0] > 1.2 || sum_maxVal[2] > 1.2){
		if(sum_maxVal[0] < sum_maxVal[2]){
			init_a+=3;
		}
		else if(sum_maxVal[0] > sum_maxVal[2]){
			init_a-=3;
		}
	}*/

	// 探索結果の場所に矩形を描画
	/*rectangle(dst0, roi_rect1[0], cv::Scalar(255, 0, 0), 3);
	rectangle(dst0, roi_rect2[0], cv::Scalar(0, 255, 0), 3);
	rectangle(dst0, sum_roi_rect[0], cv::Scalar(0, 0, 255), 3);
	rectangle(dst1, roi_rect1[1], cv::Scalar(255, 0, 0), 3);
	rectangle(dst1, roi_rect2[1], cv::Scalar(0, 255, 0), 3);
	rectangle(dst1, sum_roi_rect[1], cv::Scalar(0, 0, 255), 3);
	rectangle(dst2, roi_rect1[2], cv::Scalar(255, 0, 0), 3);
	rectangle(dst2, roi_rect2[2], cv::Scalar(0, 255, 0), 3);
	rectangle(dst2, sum_roi_rect[2], cv::Scalar(0, 0, 255), 3);
	
	imshow("matchCSV0", dst0);
	imshow("matchCSV1", dst1);
	imshow("matchCSV2", dst2);
	imshow("mC_sum_result0", sum_result[0]);
	imshow("mC_sum_result1", sum_result[1]);
	imshow("mC_sum_result2", sum_result[2]);*/

	//cout << init_a << ", " << sum_maxVal[0] << ", " << sum_maxVal[1] << ", " << sum_maxVal[2] << endl;
	int max_i = 0;
	double max_v = 0;
	for(int i = 0; i < ai; i++){
		cout << i << ":" << sum_maxVal[i] << endl; 
		if(max_v <= sum_maxVal[i]){
			max_i = i;
			max_v = sum_maxVal[i];
		}
	}

	

int64 end = cv::getTickCount();
double elapsedMsec = (end - start) * 1000 / cv::getTickFrequency();
std::cout << elapsedMsec << "ms" << std::endl;
	
	cout << max_i * 3 + 45 << ":" << max_v << endl; 

	rectangle(dst0, roi_rect1[max_i], cv::Scalar(255, 0, 0), 3);
	rectangle(dst0, roi_rect2[max_i], cv::Scalar(0, 255, 0), 3);
	rectangle(dst0, sum_roi_rect[max_i], cv::Scalar(0, 0, 255), 3);
	putText(dst0, to_string(max_i * 3 + 45), cv::Point(50,50), cv::FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(0,0,200), 2, CV_AA);
	imshow("matchCSV0", dst0);
	imshow("mC_sum_result0", sum_result[max_i]);
	imshow("mC_temp", templatesR[max_i * 3 + 45]);
	
	int aa = 0;

	Mat dist = templates[aa].clone();
	rectangle(dist, Rect(tm[aa].p1.x, tm[aa].p1.y, mouseSize.width, mouseSize.height), cv::Scalar(255, 0, 0), 3);
	rectangle(dist, Rect(tm[aa].p2.x, tm[aa].p2.y, mouseSize.width, mouseSize.height), cv::Scalar(0, 255, 0), 3);
	
	imwrite("result\\gazo\\data_RM_LM_0.jpg", dist);
	aa = 45;

	dist = templates[aa].clone();
	rectangle(dist, Rect(tm[aa].p1.x, tm[aa].p1.y, mouseSize.width, mouseSize.height), cv::Scalar(255, 0, 0), 3);
	rectangle(dist, Rect(tm[aa].p2.x, tm[aa].p2.y, mouseSize.width, mouseSize.height), cv::Scalar(0, 255, 0), 3);
	
	imwrite("result\\gazo\\data_RM_LM_45.jpg", dist);
	aa = 90;

	dist = templates[aa].clone();
	rectangle(dist, Rect(tm[aa].p1.x, tm[aa].p1.y, mouseSize.width, mouseSize.height), cv::Scalar(255, 0, 0), 3);
	rectangle(dist, Rect(tm[aa].p2.x, tm[aa].p2.y, mouseSize.width, mouseSize.height), cv::Scalar(0, 255, 0), 3);
	
	imwrite("result\\gazo\\data_RM_LM_90.jpg", dist);
	aa = 135;

	dist = templates[aa].clone();
	rectangle(dist, Rect(tm[aa].p1.x, tm[aa].p1.y, mouseSize.width, mouseSize.height), cv::Scalar(255, 0, 0), 3);
	rectangle(dist, Rect(tm[aa].p2.x, tm[aa].p2.y, mouseSize.width, mouseSize.height), cv::Scalar(0, 255, 0), 3);
	
	imwrite("result\\gazo\\data_RM_LM_135.jpg", dist);
	aa = 180;

	dist = templates[aa].clone();
	rectangle(dist, Rect(tm[aa].p1.x, tm[aa].p1.y, mouseSize.width, mouseSize.height), cv::Scalar(255, 0, 0), 3);
	rectangle(dist, Rect(tm[aa].p2.x, tm[aa].p2.y, mouseSize.width, mouseSize.height), cv::Scalar(0, 255, 0), 3);
	
	imwrite("result\\gazo\\data_RM_LM_180.jpg", dist);
	waitKey(0);
}

void TemplateMatching::importCSV(string csv_path){
	ifstream ifs(csv_path);
	string str,hoge,huga;

	if (!ifs)
	{
		cout << "Error:Input data file not found" << endl;
		return;
	}

	while(getline(ifs, str)){
		string token;
		stringstream ss, ss2;
 
		istringstream stream(str);
		int i;
		if(getline(stream, token, ','))
		{
			i = stoi(token);
			tm[i].index = i;
		}
		if(getline(stream, token, ','))
		{
			tm[i].path = token;
		}
		if(getline(stream, token, ','))
		{
			tm[i].p1.x = stoi(token);
		}
		if(getline(stream, token, ','))
		{
			tm[i].p1.y = stoi(token);
		}
		if(getline(stream, token, ','))
		{
			tm[i].p2.x = stoi(token);
		}
		if(getline(stream, token, ','))
		{
			tm[i].p2.y = stoi(token);
		}

		cout << "index : " << tm[i].index << ", p1: " << tm[i].p1.x << "," << tm[i].p1.y << ", p2: " << tm[i].p2.x << "," << tm[i].p2.y << endl;
	}
	return;
}

Mat TemplateMatching::matching(Mat src, Mat temp, int flg){
		Mat gImg,searchImg,result_img;
		tempType = 1;
		if (tempType == 1){
			//グレースケール
			cvtColor(src, gImg, CV_RGB2GRAY);
			cvtColor(gImg, searchImg, CV_GRAY2RGB);
		}
		else if (tempType == 0){
			//グレースケール
			cvtColor(src, gImg, CV_RGB2GRAY);
			cvtColor(gImg, searchImg, CV_GRAY2RGB);
		}
		else if (tempType == 2){
			//二値化
			cvtColor(src, gImg, CV_RGB2GRAY);
			cv::threshold(gImg, searchImg, 85, 255, CV_THRESH_BINARY);
			cvtColor(searchImg, searchImg, CV_GRAY2RGB);
		}
		else if (tempType == 3){
			searchImg = src.clone();
		}
		cv::matchTemplate(searchImg, temp, result_img, flg);

		return result_img;
}

/////////////////二つのテンプレで最大のところを探す
Mat TemplateMatching::sumMatchingResult(Mat result_img1, Mat result_img2, Vec2i vec){
	Mat sum_result_img;
	Point maxp;
	double max1 = 0;
	int vx = vec[0];
	int vy = vec[1];
	sum_result_img = Mat(cv::Size(result_img1.cols - abs(vx), result_img1.rows - abs(vy)), CV_32F, cv::Scalar::all(0));

	for (int y = 0; y < result_img1.rows - abs(vy) - 1; ++y)
	{
		for (int x = 0; x < result_img1.cols - abs(vx); ++x)
		{
			float p, p2;
			if (vy > 0)
			{
				if (vx > 0)
				{
					if (result_img1.data == NULL){
						cout << " null yade" << endl;
					}
					//cout << x << "," << y << ":" << vx << "," << vy << endl;
					p = result_img1.at<float>(y, x);
					p2 = result_img2.at<float>(y + vy, x + vx);
				}
				else
				{
					p = result_img1.at<float>(y, x - vx);
					p2 = result_img2.at<float>(y + vy, x);
				}
			}
			else
			{
				if (vx > 0)
				{
					p = result_img1.at<float>(y - vy, x);
					p2 = result_img2.at<float>(y, x + vx);
				}
				else
				{
					p = result_img1.at<float>(y - vy, x - vx);
					p2 = result_img2.at<float>(y, x);
				}
			}

			float ps;

			ps = p + p2;
			sum_result_img.at<float>(y, x) = ps;

			//cout << p << "," << p2 << "," << ps << endl;
		}
		//cout << y << ":" << result_img.rows << ":" << max1 << endl;
	}
	return sum_result_img;
}

Rect TemplateMatching::maxRectSumResult(Mat sum_result, Vec2i vec, double &max_val){
	
	int vx = vec[0];
	int vy = vec[1];

	cv::Rect roi_rect(0, 0, mouseSize.width + abs(vx), mouseSize.height + abs(vy));
	cv::Point max_pt;
	double maxVal;
	cv::minMaxLoc(sum_result, NULL, &maxVal, NULL, &max_pt);
	max_val = maxVal;

	int mpx, mpy;
	if (vx > 0)
	{
		if ( vy > 0)
		{
			mpx = max_pt.x;
			mpy = max_pt.y;
		}
		else
		{
			mpx = max_pt.x;
			mpy = max_pt.y - vy;
		}
	}
	if (vx < 0)
	{
		if ( vy > 0)
		{
			mpx = max_pt.x - vx;
			mpy = max_pt.y;
		}
		else
		{
			mpx = max_pt.x - vx;
			mpy = max_pt.y - vy;
		}
	}
	Point mp(mpx, mpy);
	roi_rect.x = max_pt.x;
	roi_rect.y = max_pt.y;

	return roi_rect; 
}

Rect TemplateMatching::maxRectResult(Mat result_img, double &max_val){// 最大のスコアの場所を探す
	Rect roi_rect(0, 0, mouseSize.width, mouseSize.height);
	Point max_pt;
	double maxVal;
	minMaxLoc(result_img, NULL, &maxVal, NULL, &max_pt);
	roi_rect.x = max_pt.x;
	roi_rect.y = max_pt.y;
	return roi_rect;
}
