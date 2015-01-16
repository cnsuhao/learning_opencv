#include "ColorHistogram.h"


ColorHistogram::ColorHistogram()
{
	// Prepare arguments for a color histogram
	histSize[0] = histSize[1] = histSize[2] = 256;
	hrange[0] = 0.0;    // BRG range
	hrange[1] = 255.0;
	ranges[0] = hrange; // all channels have the same range 
	ranges[1] = hrange;
	ranges[2] = hrange;
	channels[0] = 0;		// the three channels 
	channels[1] = 1;
	channels[2] = 2;

	dims = 3;

	uniform = true;
	accumulate = false;

}


ColorHistogram::~ColorHistogram()
{
}


SparseMat ColorHistogram::getSparseHistogram(InputArray image){
	SparseMat hist;

	hrange[0] = 0.0f;
	hrange[1] = 255.0f;
	channels[0] = 0;
	channels[1] = 1;
	channels[2] = 2;
	dims = 3;

	calcHist(&image.getMat(),
		1,
		channels,
		Mat(),
		hist,
		dims,
		histSize,
		ranges,
		uniform,
		accumulate);
	return hist;
}

MatND ColorHistogram::getabHistogram(InputArray image){
	MatND hist;
	Mat lab;

	hrange[0] = -128.0f;
	hrange[1] = 127.0f;
	channels[0] = 1;
	channels[1] = 2;
	dims = 2;
	//dims = 1;

	cvtColor(image, lab, COLOR_BGR2Lab);
	//imshow("labROI", lab);

	calcHist(&lab,
		1,
		channels,
		Mat(),
		hist,
		dims,
		histSize,
		ranges,
		uniform,
		accumulate);

	//imshow("lab hist", hist);
	return hist;
}

MatND ColorHistogram::getHueHistogram(InputArray image){
	MatND hist;
	Mat hue;
	hrange[0] = 0.0f;
	hrange[1] = 180.0f;
	channels[0] = 0;
	dims = 1;
	cvtColor(image, hue, COLOR_BGR2HSV);
	//imshow("hueROI", hue);
	calcHist(&hue,
		1,
		channels,
		Mat(),
		hist,
		dims,
		histSize,
		ranges,
		uniform,
		accumulate);
	return hist;
}

// Computes the histogram.
cv::MatND ColorHistogram::getHistogram(const cv::Mat &image) {

	cv::MatND hist;

	// BGR color histogram
	hrange[0] = 0.0;    // BRG range
	hrange[1] = 255.0;
	channels[0] = 0;		// the three channels 
	channels[1] = 1;
	channels[2] = 2;

	// Compute histogram
	cv::calcHist(&image,
		1,			// histogram of 1 image only
		channels,	// the channel used
		cv::Mat(),	// no mask is used
		hist,		// the resulting histogram
		3,			// it is a 3D histogram
		histSize,	// number of bins
		ranges		// pixel value range
		);

	return hist;
}
