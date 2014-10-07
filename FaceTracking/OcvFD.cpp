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
	std::vector<cv::KeyPoint> keypoint1, keypoint2;
	detector->detect(img1, keypoint1);
	detector->detect(img2, keypoint2);

	// 特徴記述
	cv::Ptr<cv::DescriptorExtractor> extractor = cv::DescriptorExtractor::create(descriptorExtractorName);
	cv::Mat descriptor1, descriptor2;
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