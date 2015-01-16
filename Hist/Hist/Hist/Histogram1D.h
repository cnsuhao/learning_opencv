#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>

using namespace cv;
using namespace std;

class Histogram1D
{
private:
// 	calcHist(const Mat* images, int nimages, const int* channels, InputArray mask, OutputArray hist, int dims, const int* histSize
// 		, const float** ranges, bool uniform /* = true */, bool accumulate /* = false */);
// 	
	/**/
	int channels[1];
	int dims;
	int histSize[1];
	float hrange[2];
	const float *ranges[1];
	bool uniform;
	bool accumulate;

public:
	Histogram1D();
	~Histogram1D();
	MatND getHistogram(InputArray image);
	Mat getHistogramImage(InputArray image);
	Mat applylookup(InputArray image, InputArray lookup);
	Mat stretch(InputArray image, int minVal = 0);
	Mat equalize(InputArray image){
		Mat result;
		equalizeHist(image, result);
		return result;
	}
};



