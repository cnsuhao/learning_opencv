#pragma once

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include "colorhistogram.h"

class ImageComparator
{
private:
	Mat reference;
	Mat input;
	MatND refHist;
	MatND inputHist;
	int div;

	ColorHistogram hist;

public:
	ImageComparator();
	~ImageComparator();
	void setColorReduce(int val);
	int getColorReduce();
	void setReferenceImage(const Mat image);
	double compareImage(const Mat iamge);
};

