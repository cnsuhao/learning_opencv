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
	/*����һ��Ҫͳ�Ƶ�ԭͼ�񣬶����������ͼ���ֱ��ͼ������mask�趨����
	�ģ����ͼ���壺ͼ���ά���� ����ÿһλ�������С�����ߣ��Ƿ��һ����
	�ˣ���*/
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
	
	//��ȡ���ֵ����Сֵ
	double minVal;
	double maxVal;
	int imin[3];
	int imax[3];
	minMaxIdx(hist, &minVal, &maxVal, imin, imax, noArray());

	/*��ֱ��ͼ�����ݻ��Ƶ�histImage
	����ͼ�����ߵ���histSize��0.9
	ÿ���Ҷȼ���ͳ������Ӧһ��ֱ��
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


