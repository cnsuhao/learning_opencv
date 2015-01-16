#include <iostream>
#include "Histogram1D.h"
#include "ColorHistogram.h"
#include "ObjectFinder.h"
#include "imageComparator.h"

using namespace std;

bool testHistogram(){
	Mat image = imread("group.jpg", 0);
	if (image.data == NULL){
		printf("error in read Image\n");
		return true;
	}
	Histogram1D histogram1d = Histogram1D();
	Mat result = histogram1d.stretch(image, 300);

	Mat thresholded1;
	Mat thresholded2;
	threshold(image, thresholded1, 60, 255, THRESH_BINARY);//有种增强对比度的感觉
	//threshold(image, thresholded2, 150, 255, THRESH_BINARY);

	imshow("image", image);
	imshow("result", result);
	imshow("threshold", thresholded1);
	imshow("histogramImage", histogram1d.getHistogramImage(image));
	imshow("histogram1d", histogram1d.getHistogramImage(result));
	imshow("histogramthreshold", histogram1d.getHistogramImage(thresholded1));
	imshow("eqimage", histogram1d.equalize(image));
	imshow("eqimagehist", histogram1d.getHistogramImage(histogram1d.equalize(image)));
	return false;
}

bool testObjectFider_InGrayImage(){
	// Read input image
	cv::Mat image = cv::imread("waves.jpg", 0);
	if (!image.data)
	return 0;

	// define image ROI
	cv::Mat imageROI;
	imageROI = image(cv::Rect(360, 55, 40, 50)); // Cloud region

	// Display reference patch
	cv::namedWindow("Reference");
	cv::imshow("Reference", imageROI);

	// Find histogram of reference
	Histogram1D h;
	cv::MatND hist = h.getHistogram(imageROI);
	cv::namedWindow("Reference Hist");
	cv::imshow("Reference Hist", h.getHistogramImage(imageROI));

	// Create the objectfinder
	ObjectFinder finder;
	finder.setHistogram(hist);

	finder.setThreshold(-1.0f);

	// Get back-projection
	cv::Mat result1;
	result1 = finder.find(image);

	// Create negative image and display result
	cv::Mat tmp;
	result1.convertTo(tmp, CV_8U, -1.0, 255.0);
	cv::namedWindow("Backprojection result");
	cv::imshow("Backprojection result", tmp);

	// Get binary back-projection
	finder.setThreshold(0.12f);
	result1 = finder.find(image);

	// Draw a rectangle around the reference area
	cv::rectangle(image, cv::Rect(360, 55, 40, 50), cv::Scalar(0, 0, 0));

	// Display image
	cv::namedWindow("Image");
	cv::imshow("Image", image);

	// Display result
	cv::namedWindow("Detection Result");
	cv::imshow("Detection Result", result1);

	// Second test image
	cv::Mat image2 = cv::imread("dog.jpg", 0);
	cv::Mat result2;
	result2 = finder.find(image2);

	// Display result
	cv::imshow("image2", image2);
	cv::namedWindow("Result (2)");
	cv::imshow("Result (2)", result2);
	return 0;
}

bool testColorHistogram(){
	//Mat image = imread("group.jpg", 0);
	Mat image = imread("waves.jpg");
	if (image.data == NULL){
		printf("error in read Image\n");
		return true;
	}
	//Histogram1D histogram1d = Histogram1D();
	//Mat histImage = histogram1d.gethistogramImage(image);
	//Mat result = histogram1d.stretch(image, 5);

	ColorHistogram colorHistogram = ColorHistogram();
	SparseMat hist = colorHistogram.getSparseHistogram(image);

	Mat thresholded1;
 	//Mat thresholded2;
	cv::threshold(image, thresholded1, 60, 255, THRESH_BINARY);//有种增强对比度的感觉
	// 	threshold(image, thresholded2, 150, 255, THRESH_BINARY);
	
	cv::imshow("image", image);
	cv::imshow("thresholded1", thresholded1);
	return false;
}

bool testObjectFider_InRGBImage(){
	
	Mat imageROI;
	Mat result1;
	Mat result2;
	ObjectFinder finder;

	// Load color image
	ColorHistogram hc;
	cv::Mat color = cv::imread("waves.jpg");
	color = hc.colorReduce(color, 32);
	cv::Mat showImage = color.clone();

	rectangle(showImage, cv::Rect(360, 55, 40, 50), Scalar(0, 0, 255)),
	imageROI = color(cv::Rect(360, 55, 40, 50)); // blue sky area
	cv::imshow("imageROI", imageROI);

	// Get 3D color histogram
	cv::MatND shist = hc.getHistogram(imageROI);
	// Histograms with SparseMat does not work with OpenCV2.1
	// cv::SparseMat shist= hc.getSparseHistogram(imageROI);

	
	finder.setHistogram(shist);
	finder.setThreshold(0.05f);

	// Get back-projection of color histogram
	result1 = finder.find(color);

	cv::namedWindow("Color Backproject Result");
	cv::imshow("Color Backproject Result", result1);
	cv::namedWindow("Color Image");
	cv::imshow("Color Image", showImage);

	/////////////////////////////////////////////////////////////////////
	// Second color image
	cv::Mat color2 = cv::imread("dog.jpg");
	color2 = hc.colorReduce(color2, 32);

	// Get back-projection of color histogram
	result2 = finder.find(color2);

	cv::imshow("dog Image", color2);
	cv::namedWindow("Result color (2)");
	cv::imshow("Result color (2)", result2);

	cv::waitKey();
	return 0;
}

bool testObjectFider_InHsvImage(){
	
	Mat imageROI;
	
	Mat result1;
	
	ObjectFinder finder;
	

	// Load color image
	ColorHistogram hc;
	cv::Mat color = cv::imread("waves.jpg");
	color = hc.colorReduce(color, 32);

	// Get Hue color histogram
	color = cv::imread("waves.jpg");
	imageROI = color(cv::Rect(360, 55, 40, 50)); // blue sky area
	Mat colorhist = hc.getHueHistogram(imageROI);

	finder.setHistogram(colorhist);
	finder.setThreshold(0.3f);

	//finder.setHistogram(colorhist);
	//finder.setThreshold(0.05f);

	// Convert to HSV space
	cv::Mat hsv;
	cv::Mat showImage;
	cv::cvtColor(color, hsv, cv::COLOR_BGR2HSV);
	showImage = hsv.clone();

	finder.setHistogram(colorhist);
	finder.setThreshold(0.3f);

	// Get back-projection of hue histogram
	int ch[1] = {0};
	result1 = finder.find(hsv, 0.0f, 180.0f, ch, 1);

	cv::namedWindow("Result Hue (1)");
	cv::imshow("Result Hue (1)", result1);
	rectangle(showImage,cv::Rect(360, 55, 40, 50),Scalar(0,0,255)),
	cv::imshow("hsv1", showImage);

	////////////////////////////////////////////////////////////////
	// Second color image
	Mat imageROI2;
	Mat result2;
	ObjectFinder finder2;
	ColorHistogram hc2;
	
	Mat hsv2;
	Mat color2 = cv::imread("dog.jpg");
	color2 = hc2.colorReduce(color2, 32);

	imageROI2 = color2(cv::Rect(0, 0, 165, 75)); // blue sky area
	Mat colorhist2 = hc2.getHueHistogram(imageROI2);

	finder2.setHistogram(colorhist2);
	finder2.setThreshold(0.3f);

	cv::cvtColor(color2, hsv2, cv::COLOR_BGR2HSV);
	imshow("hsv2", hsv2);

	int ch2[1] = { 0 };
	result2 = finder2.find(hsv2, 0.0f, 180.0f, ch2, 1);

	cv::namedWindow("Result Hue (2)");
	cv::imshow("Result Hue (2)", result2);

	cv::waitKey();
	return 0;
}

bool testObjectFider_InLabImage(){
	Mat imageROI;
	Mat result1;
	Mat result2;
	ObjectFinder finder;

	// Load color image
	ColorHistogram hc;
	cv::Mat color = cv::imread("waves.jpg");
	color = hc.colorReduce(color, 32);
	//cv::namedWindow("Color Image");
	//cv::imshow("Color Image", color);

	// Get ab color histogram
	//imageROI = color(cv::Rect(0, 0, 165, 75)); // blue sky area
	imageROI = color(cv::Rect(360, 55, 40, 50));
	cv::MatND colorhist = hc.getabHistogram(imageROI);

	finder.setHistogram(colorhist);
	finder.setThreshold(0.05f);


	// Convert to Lab space
	cv::Mat lab;
	cv::cvtColor(color, lab, cv::COLOR_BGR2Lab);
	Mat showImage = lab.clone();

	// Get back-projection of ab histogram
	//int ch[2] = { 1, 2 };
	int ch[2] = { 1, 2 };
	result1 = finder.find(lab, -128.0f, 127.0f, ch, 2);
	rectangle(showImage, cv::Rect(360, 55, 40, 50), Scalar(0, 0, 255));

	cv::namedWindow("Result ab (1)");
	cv::imshow("Result ab (1)", result1);
	imshow("lab1", showImage);

	/////////////////////////////////////////////////////////////
	// Second color image
 	cv::Mat color2 = cv::imread("dog.jpg");
 	color2 = hc.colorReduce(color2, 32);
 
 	cv::namedWindow("Color Image (2)");
 	cv::imshow("Color Image (2)", color2);
 
	Mat lab2;
 	cv::cvtColor(color2, lab2, cv::COLOR_BGR2Lab);
 	imshow("lab2", lab2);
 
 	result2 = finder.find(lab2, -128.0f, 127.0f, ch, 2);
 
 	cv::namedWindow("Result ab (2)");
 	cv::imshow("Result ab (2)", result2);

	waitKey();
	return 0;
}

void testFinder(){
	// Read reference image
	cv::Mat image = cv::imread("baboon1.jpg");
	if (!image.data)
		return ;

	// Define ROI
	cv::Mat imageROI = image(cv::Rect(110, 260, 35, 40));
	cv::rectangle(image, cv::Rect(110, 260, 35, 40), cv::Scalar(0, 0, 255));

	// Display image
	cv::namedWindow("Image");
	cv::imshow("Image", image);

	// Get the Hue histogram
	int minSat = 65;
	ColorHistogram hc;
	cv::MatND colorhist = hc.getHueHistogram(imageROI);

	ObjectFinder finder;
	finder.setHistogram(colorhist);
	finder.setThreshold(0.2f);

	// Convert to HSV space
	cv::Mat hsv;
	cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

	// Split the image
	vector<cv::Mat> v;
	cv::split(hsv, v);

	// Eliminate pixels with low saturation
	cv::threshold(v[1], v[1], minSat, 255, cv::THRESH_BINARY);
	cv::namedWindow("Saturation");
	Mat mergeResult;
	merge(v, mergeResult);
	cv::imshow("merge", mergeResult);
	cv::imshow("hsv", hsv);

	// Get back-projection of hue histogram
	int ch[1] = { 0 };
	cv::Mat result = finder.find(hsv, 0.0f, 180.0f, ch, 1);

	cv::namedWindow("Result Hue");
	cv::imshow("Result Hue", result);

	cv::bitwise_and(result, v[1], result);
	cv::namedWindow("Result Hue and");
	cv::imshow("Result Hue and", result);

	// Second image
	image = cv::imread("baboon3.jpg");

	// Display image
	cv::namedWindow("Image 2");
	cv::imshow("Image 2", image);

	// Convert to HSV space
	cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

	// Split the image
	cv::split(hsv, v);

	cout << hsv.elemSize() << endl;

	// Eliminate pixels with low saturation
	cv::threshold(v[1], v[1], minSat, 255, cv::THRESH_BINARY);
	cv::namedWindow("Saturation");
	cv::imshow("Saturation", v[1]);

	// Get back-projection of hue histogram
	result = finder.find(hsv, 0.0f, 180.0f, ch, 1);

	cv::namedWindow("Result Hue");
	cv::imshow("Result Hue", result);

	// Eliminate low stauration pixels
	cv::bitwise_and(result, v[1], result);
	cv::namedWindow("Result Hue and");
	cv::imshow("Result Hue and", result);

	// Get back-projection of hue histogram
	finder.setThreshold(-1.0f);
	result = finder.find(hsv, 0.0f, 180.0f, ch, 1);
	cv::bitwise_and(result, v[1], result);
	cv::namedWindow("Result Hue and raw");
	cv::imshow("Result Hue and raw", result);

	
	cv::Rect rect(110, 260, 35, 40);
	cv::rectangle(image, rect, cv::Scalar(0, 0, 255));

	cv::TermCriteria criteria(cv::TermCriteria::MAX_ITER, 10, 0.01);
	cout << "meanshift= " << cv::meanShift(result, rect, criteria) << endl;

	cv::rectangle(image, rect, cv::Scalar(0, 255, 0));

	// Display image
	cv::namedWindow("Image 2 result");
	cv::imshow("Image 2 result", image);

	cv::waitKey();
}

void testRetrive(){
	// Read reference image
	cv::Mat image = cv::imread("waves.jpg");
	if (!image.data)
		return ;

	// Display image
	cv::namedWindow("Query Image");
	cv::imshow("Query Image", image);

	ImageComparator c;
	c.setReferenceImage(image);

	// Read an image and compare it with reference
	cv::Mat input = cv::imread("dog.jpg");
	cout << "waves vs dog: " << c.compareImage(input) << endl;

	// Read an image and compare it with reference
	input = cv::imread("marais.jpg");
	cout << "waves vs marais: " << c.compareImage(input) << endl;

	// Read an image and compare it with reference
	input = cv::imread("bear.jpg");
	cout << "waves vs bear: " << c.compareImage(input) << endl;

	// Read an image and compare it with reference
	input = cv::imread("beach.jpg");
	cout << "waves vs beach: " << c.compareImage(input) << endl;

	// Read an image and compare it with reference
	input = cv::imread("polar.jpg");
	cout << "waves vs polar: " << c.compareImage(input) << endl;

	// Read an image and compare it with reference
	input = cv::imread("moose.jpg");
	cout << "waves vs moose: " << c.compareImage(input) << endl;

	// Read an image and compare it with reference
	input = cv::imread("lake.jpg");
	cout << "waves vs lake: " << c.compareImage(input) << endl;

	// Read an image and compare it with reference
	input = cv::imread("fundy.jpg");
	cout << "waves vs fundy: " << c.compareImage(input) << endl;

	cv::waitKey();
	return ;
}

int main()
{
	//testHistogram();
	//testColorHistogram();
	//testObjectFider_InGrayImage();
	//testObjectFider_InRGBImage();
	//testObjectFider_InHsvImage();
	//testObjectFider_InLabImage();
	testFinder();
	//testRetrive();
	waitKey(0);
	return 0;
}