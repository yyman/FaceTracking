#include "VgaCap.h"

VgaCap::VgaCap(){
}

VgaCap::~VgaCap(void){
}

void VgaCap::cap( VideoCapture frame){
	// 矢印キー
	const int CV_WAITKEY_CURSORKEY_TOP    = 2490368;
	const int CV_WAITKEY_CURSORKEY_BOTTOM = 2621440;
	const int CV_WAITKEY_CURSORKEY_RIGHT  = 2555904;
	const int CV_WAITKEY_CURSORKEY_LEFT   = 2424832;

	bool caploop = true;
	Mat capImg,showImg;
	unsigned char key = 0;
	int i = 45;

	char path[] = "result\\model\\test";
	if( PathFileExists( path ) && PathIsDirectory( path ) ){
		// 指定されたパスにファイルが存在、かつディレクトリである
		cout << "[" << path << "] is existed" << endl;
	}else if( !PathFileExists( path ) ){
		// 指定されたパスが存在しない
		_mkdir(path);
		cout<<"create directry [" << path << "]" << endl;
	}

	while(caploop){
		frame >> capImg;
		capImg.copyTo(showImg);
		ostringstream oss;
		oss << "\\" << i << ".jpg";
		cv::putText(showImg, oss.str(), cv::Point(50,50), cv::FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(0,0,200), 2, CV_AA);
		imshow("capImg", showImg);
		key = waitKey(5);

		switch(key){
		case 27: 
			caploop = false;
			break;
		case 32:
			imwrite(path + oss.str(), capImg);
			break;
		case 'w':
			i += 3;
			cout<<i<<endl;
			break;
		case 's':
			i -= 3;
			cout<<i<<endl;
			break;
		default:
			//cout<<(char)key<<endl;
			break;
		}
	}
}
