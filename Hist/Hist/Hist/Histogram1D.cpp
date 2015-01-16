#include "Histogram1D.h"


Histogram1D::Histogram1D()
{
	histSize[0] = 256;
	dims = 1;
	hrange[0] = .0f;
	hrange[1] = 255.0f;
	ranges[0] = hrange;
	uniform = true;
	accumulate = false;
	channels[0] = 0;
}


Histogram1D::~Histogram1D()
{
}

MatND Histogram1D::getHistogram(InputArray image){
	MatND hist;
	/*参数一：要统计的原图像，二：计算多少图像的直方图，三：mask设定区域
	四：输出图像，五：图像的维数， 六：每一位的最大最小数，七：是否归一化，
	八：？*/
	calcHist(&image.getMat(),		
		1,							
		channels,
		Mat(),
		hist,
		1,
		histSize,
		ranges,
		uniform,
		accumulate);
	return hist;
}

Mat Histogram1D::getHistogramImage(InputArray image){
	MatND hist;
	hist = getHistogram(image);
	
	//获取最大值、最小值
	double minVal;
	double maxVal;
	int imin[3];
	int imax[3];
	minMaxIdx(hist, &minVal, &maxVal, imin, imax, noArray());

	/*将直方图的数据绘制到histImage
	绘制图像的最高点是histSize的0.9
	每个灰度级的统计量对应一条直线
	*/
	Mat histImage;
	histImage = Mat(Size(histSize[0], histSize[0]), CV_8U, Scalar::all(0));

	int hpt = static_cast<int>(0.9f*histSize[0]);

	for (int h = 0; h < histSize[0]; h++){
		float nbins = hist.at<float>(h);
		float intensity = nbins * 1.0f / maxVal * hpt;
		line(histImage, Point(h, histSize[0]), Point(h, histSize[0] - intensity), Scalar::all(255));
	}

	cout << "minVal:" << minVal << endl;
	cout << "maxVal:" << maxVal << endl;
	cout << "imin:" << imin[0] << endl;
	cout << "imax:" << imax[0] << endl;

	return histImage;
}


Mat Histogram1D::applylookup(InputArray image, InputArray lookup){
	Mat result;
	LUT(image.getMat(), lookup.getMat(), result);
	return result;
}

Mat Histogram1D::stretch(InputArray image, int minVal /* = 0 */){
	Mat hist = getHistogram(image);

	int imin ;
	for (imin = 0; imin < histSize[0]; imin++)
	{
		if (hist.at<float>(imin) > minVal)
			break;
	}

	int imax;
	for (imax = histSize[0] - 1; imax > 0; imax--)
	{
		if (hist.at<float>(imax) > minVal)
			break;
	}

	Mat lookup;
	lookup = Mat(1, histSize[0], CV_8U);
	for (int i = 0; i < histSize[0]; i++){
		if (i < imin){
			lookup.at<uchar>(i) = 0;
		}
		else if (i > imax){
			lookup.at<uchar>(i) = 255;
		}
		else {
			lookup.at<uchar>(i) = static_cast<uchar>(255.0*(i - imin)/(imax - imin));
		}
	}

	Mat result = applylookup(image, lookup);
	return result;
}


