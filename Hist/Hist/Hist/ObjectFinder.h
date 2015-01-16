#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class ObjectFinder
{
private:
	int channels[3];
	float hrange[2];
	const float *ranges[3];
	float thresholdVal;
	MatND histogram;
	SparseMat shistogram;
	bool isSparse;
public:
	ObjectFinder();
	~ObjectFinder();
	void setHistogram(const MatND &hist);
	void setHistogram(const SparseMat &hist);
	Mat find(const Mat &image);
	Mat find(const Mat &image, float minVal, float maxVal, int *channels, int dims);
	void setThreshold(float val);
};

