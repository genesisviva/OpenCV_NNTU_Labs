#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat original;
	original = imread("shakal.jpg", cv::IMREAD_COLOR);
	Mat HSV;
	cvtColor(original, HSV, cv::COLOR_BGR2HSV);
	Mat grayscale;
	grayscale = imread("shakal.jpg", cv::IMREAD_GRAYSCALE);
	imshow("Original", original);
	imshow("HSV", HSV);
	imshow("Gray", grayscale);
	Mat binary;
	threshold(grayscale, binary, 127, 255, THRESH_BINARY);
	imshow("Binary", binary);
	Mat pictures = original;
	circle(pictures, Point(100, 160), 25, Scalar(255, 255, 50, 255), -1);
	circle(pictures, Point(195, 175), 25, Scalar(255, 255, 50, 255), -1);
	line(pictures, Point(100, 160), Point(195, 175), Scalar(255, 255, 50, 255), 3);
	rectangle(pictures, Point(100, 255), Point(110, 275), Scalar(0, 255, 255, 0), -1);
	imshow("Drawings", pictures);
	waitKey(0);
	return 0;
}