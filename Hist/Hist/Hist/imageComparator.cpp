#include "imageComparator.h"


ImageComparator::ImageComparator()
{
	this->hist = * new ColorHistogram();
	div = 32;
}


ImageComparator::~ImageComparator()
{
}


void ImageComparator::setColorReduce(int val){
	this->div = val;
}

int ImageComparator::getColorReduce(){
	return this->div;
}

void ImageComparator::setReferenceImage(const Mat image){
	reference = hist.colorReduce(image, div);
	refHist = hist.getHistogram(reference);
}

double ImageComparator::compareImage(const Mat image){
	input = hist.colorReduce(image, div);
	this->inputHist = hist.getHistogram(input);
	double familiar = compareHist(this->refHist, this->inputHist, HISTCMP_INTERSECT);
	return familiar;
}

