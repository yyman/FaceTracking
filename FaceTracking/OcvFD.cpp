#include "OcvFD.h"

OcvFD::OcvFD(){
}

OcvFD::~OcvFD(void){
}

void OcvFD::matching(
	Mat img1,			// 画像１
	Mat img2,			// 画像２
	const std::string& featureDetectorName,		// detectorType
	const std::string& descriptorExtractorName,	// descriptorExtractorType
	const std::string& descriptorMatcherName,	// descriptorMatcherType
	bool crossCheck)				// マッチング結果をクロスチェックするかどうか
{

	// SIFT・SURFモジュールの初期化
	cv::initModule_nonfree();

	// 特徴点抽出
	cv::Ptr<cv::FeatureDetector> detector = cv::FeatureDetector::create(featureDetectorName);
	printParams(detector);
	std::vector<cv::KeyPoint> keypoint1, keypoint2;
	detector->detect(img1, keypoint1);
	detector->detect(img2, keypoint2);

	// 特徴記述
	cv::Ptr<cv::DescriptorExtractor> extractor = cv::DescriptorExtractor::create(descriptorExtractorName);
	Mat descriptor1(1, 1, CV_8UC1), descriptor2(1, 1, CV_8UC1);
	extractor->compute(img1, keypoint1, descriptor1);
	extractor->compute(img2, keypoint2, descriptor2);

	Mat keyout1,keyout21,keyout22;
	//drawKeypoints(img1, keypoint1, keyout1);
	//drawKeypoints(img2, keypoint2, keyout2);
	try{
	drawKeypoints(img1, keypoint1, keyout1, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	}
	catch(Exception& e){
		cout<<e.msg<<endl;
		drawKeypoints(img1, keypoint1, keyout1);
	}
	drawKeypoints(img2, keypoint2, keyout21, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(img2, keypoint2, keyout22);

	imshow(featureDetectorName+"_"+descriptorExtractorName+"_"+descriptorMatcherName+"key1", keyout1);
	imshow(featureDetectorName+"_"+descriptorExtractorName+"_"+descriptorMatcherName+"key21", keyout21);
	imshow(featureDetectorName+"_"+descriptorExtractorName+"_"+descriptorMatcherName+"key22", keyout22);

	// マッチング
	cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(descriptorMatcherName);
	std::vector<cv::DMatch> dmatch;
	if (crossCheck)
	{
		// クロスチェックする場合
		std::vector<cv::DMatch> match12, match21;
		matcher->match(descriptor1, descriptor2, match12);
		matcher->match(descriptor2, descriptor1, match21);
		for (size_t i = 0; i < match12.size(); i++)
		{
			cv::DMatch forward = match12[i];
			cv::DMatch backward = match21[forward.trainIdx];
			if (backward.trainIdx == forward.queryIdx)
				dmatch.push_back(forward);
		}
	}
	else
	{
		// クロスチェックしない場合
		matcher->match(descriptor1, descriptor2, dmatch);
	}

	// マッチング結果の表示
	cv::Mat out;
	cv::drawMatches(img1, keypoint1, img2, keypoint2, dmatch, out);
	cv::imshow(featureDetectorName+"_"+descriptorExtractorName+"_"+descriptorMatcherName, out);
	while (cv::waitKey(1) == -1);
}

string OcvFD::getNameOfType(int argType)
{
	switch(argType)
	{
		case Param::INT: return "integer";
		case Param::SHORT: return "short";
		case Param::BOOLEAN: return "boolean";
		case Param::REAL: return "double";
		case Param::STRING: return "string";
		case Param::MAT: return "cv::Mat";
		case Param::MAT_VECTOR: return "std::vector<cv::Mat>";
		case Param::ALGORITHM: return "algorithm";
		case Param::FLOAT: return "float";
		case Param::UNSIGNED_INT: return "unsigned int";
		case Param::UINT64: return "unsigned int64";
		case Param::UCHAR: return "unsigned char";
		default: CV_Error(CV_StsBadArg, "Wrong argument type");
	}
	return "";
}

void OcvFD::printParams(const Algorithm* algo)
{
	std::cout << "Parameters of " << algo->name() << std::endl;
	std::vector<std::string> names;
	algo->getParams(names);
	for (int i = 0; i < names.size(); ++ i)
	{
		int type = algo->paramType(names[i]);
		std::cout << " - " << getNameOfType(type) << " " << names[i] << " = ";
		if( type == Param::INT )
			std::cout << algo->get<int>(names[i]) << std::endl;
		else if( type == Param::BOOLEAN )
			std::cout << algo->get<bool>(names[i]) << std::endl;
		else if( type == Param::SHORT )
			std::cout << (int)algo->get<short>(names[i]) << std::endl;
		else if( type == Param::REAL )
			std::cout << algo->get<double>(names[i]) << std::endl;
		else if( type == Param::STRING )
			std::cout << algo->get<string>(names[i]) << std::endl;
		else if( type == Param::MAT )
			std::cout << algo->get<Mat>(names[i]) << std::endl;
		else if( type == Param::MAT_VECTOR )
		{
			std::vector<Mat> mats = algo->get<vector<Mat>>(names[i]);
			for (int j = 0; j < mats.size(); ++ j)
				std::cout << mats[j] << std::endl;
			std::cout << std::endl;
		}
		else if( type == Param::ALGORITHM )
		{
			Ptr<Algorithm> nestedAlgo = algo->get<Algorithm>(names[i]);
			std::cout << nestedAlgo->name() << std::endl;
		}
		else if( type == Param::FLOAT)
			std::cout << (float)algo->getDouble(names[i]) << std::endl;
		else if( type == Param::UNSIGNED_INT)
			std::cout << (unsigned int)algo->getInt(names[i]) << std::endl;
		else if( type == Param::UINT64)
			std::cout << (unsigned long long)algo->getInt(names[i]) << std::endl;
		else if( type == Param::UCHAR)
			std::cout << (unsigned char)algo->getInt(names[i]) << std::endl;
		else
		{
			string msg = format("unknown/unsupported type of '%s' parameter == %d", names[i].c_str(), type);
			CV_Error( CV_StsUnsupportedFormat, msg.c_str());
		}
	}
}