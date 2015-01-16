#include "ObjectFinder.h"


ObjectFinder::ObjectFinder()
{
	ranges[0] = hrange;
	ranges[1] = hrange;
	ranges[2] = hrange;
	thresholdVal = 0.1f;
}


ObjectFinder::~ObjectFinder()
{
}

void ObjectFinder::setThreshold(float val){
	this->thresholdVal = val;
}

void ObjectFinder::setHistogram(const MatND &hist){
	isSparse = false;
	histogram = hist;
	normalize(histogram, histogram, 1.0);
}


void ObjectFinder::setHistogram(const SparseMat &hist){
	isSparse = true;
	shistogram = hist;
	normalize(shistogram, shistogram, 1.0, NORM_L2);
}

Mat ObjectFinder::find(const Mat &image){
	cv::Mat result;

	hrange[0] = 0.0;	// range [0,255]
	hrange[1] = 255.0;
	channels[0] = 0;		// the three channels 
	channels[1] = 1;
	channels[2] = 2;
	if (isSparse == false)
	{
		/*源图像，计算图像的数目，通道，直方图，反应射结果，每个通道取值范围，缩放比例*/
		calcBackProject(&image,
			1,
			channels,
			histogram,
			result,
			ranges,
			255.0);
	} 
	else
	{
		calcBackProject(&image,
			1,
			channels,
			shistogram,
			result,
			ranges,
			255.0);
	}

	if (thresholdVal > 0.0)
	{
		//二值化，使结果更清晰
		threshold(result, result, 255.0*thresholdVal, 255.0, THRESH_BINARY);
	}
	return result;
}


Mat ObjectFinder::find(const Mat &image, float minVal, float maxVal, int * channels, int dims){
	cv::Mat result;
	hrange[0] = minVal;
	hrange[1] = maxVal;

	for (int i = 0; i < dims; i++)
	{
		channels[i] = channels[i];
	}
	if (isSparse == false)
	{
		/*源图像，计算图像的数目，通道，直方图，反应射结果，每个通道取值范围，缩放比例*/
		calcBackProject(&image,
			1,
			channels,
			histogram,
			result,
			ranges,
			255.0);
	}
	else
	{
		calcBackProject(&image,
			1,
			channels,
			shistogram,
			result,
			ranges,
			255.0);
	}

	if (thresholdVal > 0.0)
	{
		threshold(result, result, 255.0*thresholdVal, 255.0, THRESH_BINARY);
	}
	return result;
}



