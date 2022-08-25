#include<iostream>
using namespace std;
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
using namespace cv;

Mat calcGrayHist(const Mat & image)
{
	Mat histogram = Mat::zeros(Size(256, 1), CV_32SC1);
	int rows = image.rows;
	int cols = image.cols;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			int index = int(image.at<uchar>(r, c));
			histogram.at<int>(0, index) += 1;
		}
	}
	return histogram;
}

int threshTwoPeaks(const Mat& image, Mat& thresh_out)
{
	// ����Ҷ�ֱ��ͼ
	Mat histogram = calcGrayHist(image);
	// �ҵ��Ҷ�ֱ��ͼ����ֵ��Ӧ�ĻҶ�ֵ
	Point firstPeakLoc;
	minMaxLoc(histogram, NULL, NULL, NULL, &firstPeakLoc);
	int firstPeak = firstPeakLoc.x;
	//Ѱ�һҶ�ֱ��ͼ�ĵڶ�����ֵ��Ӧ�ĻҶ�ֵ
	Mat measureDists = Mat::zeros(Size(256, 1), CV_32FC1);
	for (int k = 0; k < 256; k++)
	{
		int hist_k = histogram.at<int>(0, k);
		measureDists.at<float>(0, k) = pow(float(k - firstPeak), 2) * hist_k;
	}
	Point secondPeakLoc;
	minMaxLoc(measureDists, NULL, NULL, NULL, &secondPeakLoc);
	int secondPeak = secondPeakLoc.x;
	//�ҵ�������ֵ֮�����Сֵ��Ӧ�ĻҶ�ֵ����Ϊ��ֵ
	Point threshLoc;
	int thresh = 0;
	if (firstPeak < secondPeak) {
		minMaxLoc(histogram.colRange(firstPeak, secondPeak), NULL, NULL, &threshLoc);
		thresh = firstPeak + threshLoc.x + 1;
	}
	else {
		minMaxLoc(histogram.colRange(secondPeak, firstPeak), NULL, NULL, &threshLoc);
		thresh = secondPeak + threshLoc.x + 1;
	}
	//��ֵ�ָ�
	threshold(image, thresh_out, thresh, 255, THRESH_BINARY);
	return thresh;
}

int main(int argc, char*argv[])
{
	//����ͼ�����
	Mat image = imread("dirt.png", IMREAD_GRAYSCALE);
	if (!image.data)
	{
		cout << "û������ͼƬ" << endl;
		return -1;
	}
	//���� ������ֵ��
	Mat threshImage;
	int thresh = threshTwoPeaks(image, threshImage);
	cout << "��ֵΪ��" << thresh << endl;
	//��ʾ��ֵ��Ķ�ֵͼ
	imshow("��ֵͼ", threshImage);
	waitKey(0);
	return 0;
}

