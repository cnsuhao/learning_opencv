#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>

using namespace cv;
using namespace std;

class ColorHistogram
{
private:
	// 	calcHist(const Mat* images, int nimages, const int* channels, InputArray mask, OutputArray hist, int dims, const int* histSize
	// 		, const float** ranges, bool uniform /* = true */, bool accumulate /* = false */);
	// 	
	/**/
	int channels[3];
	int dims;
	int histSize[3];
	float hrange[2];
	const float *ranges[3];
	bool uniform;
	bool accumulate;

public:
	ColorHistogram();
	~ColorHistogram();
	cv::MatND getHistogram(const cv::Mat &image);
	SparseMat getSparseHistogram(InputArray image);
	MatND getabHistogram(InputArray image);
	MatND getHueHistogram(InputArray image);

	Mat colorReduce(const cv::Mat &image, int div = 64) {

		int n = static_cast<int>(log(static_cast<double>(div)) / log(2.0));
		// mask used to round the pixel value
		uchar mask = 0xFF << n; // e.g. for div=16, mask= 0xF0

		cv::Mat_<cv::Vec3b>::const_iterator it = image.begin<cv::Vec3b>();
		cv::Mat_<cv::Vec3b>::const_iterator itend = image.end<cv::Vec3b>();

		// Set output image (always 1-channel)
		cv::Mat result(image.rows, image.cols, image.type());
		cv::Mat_<cv::Vec3b>::iterator itr = result.begin<cv::Vec3b>();

		for (; it != itend; ++it, ++itr) {

			(*itr)[0] = ((*it)[0] & mask) + div / 2;
			(*itr)[1] = ((*it)[1] & mask) + div / 2;
			(*itr)[2] = ((*it)[2] & mask) + div / 2;
		}

		return result;
	}
};

