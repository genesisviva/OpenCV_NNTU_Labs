#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

void customBlurFunc(const Mat& source, Mat& result, double coefficient) {
    int size = floor(1 / coefficient);
    if (size / 2 == 0)
        size = size - 1;
    vector<double> kernel(size, coefficient);
    filter2D(source, result, source.depth(), kernel, Point(-1, -1));
}

void customSobelFunc(const Mat& source, Mat& result) {
    Mat grayTemp;
    cvtColor(source, grayTemp, COLOR_BGR2GRAY);
    vector<double> kernel = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };
    filter2D(grayTemp, result, source.depth(), kernel, Point(-1, -1));
}

void customFeldmanFunc(const Mat& source, Mat& result) {
    Mat grayTemp;
    cvtColor(source, grayTemp, COLOR_BGR2GRAY);
    vector<double> kernel = { 3, 10, 3, 0, 0, 0, -3, -10, -3 };
    filter2D(grayTemp, result, source.depth(), kernel, Point(-1, -1));
}

void customScharrFunc(const Mat& source, Mat& result) {
    Mat grayTemp;
    cvtColor(source, grayTemp, COLOR_BGR2GRAY);
    vector<double> kernel = { 47, 162, 47, 0, 0, 0, -47, -162, -47 };
    filter2D(grayTemp, result, source.depth(), kernel, Point(-1, -1));
}



int main()
{
    Mat original;
    original = imread("shakal.jpg", cv::IMREAD_COLOR);
    Mat blurAPI;
    Mat customBlur;
    Mat customSobel;
    Mat customFeldman;
    Mat customScharr;
    GaussianBlur(original, blurAPI, Size (7, 47), 0, 0);
    customBlurFunc(original, customBlur, 0.1);
    customSobelFunc(original, customSobel);
    customFeldmanFunc(original, customFeldman);
    customFeldmanFunc(original, customScharr);
    imshow("Original", original);
    imshow("Blured", blurAPI);
    imshow("CustomBlured", customBlur);
    imshow("CustomGrad", customSobel);
    imshow("CustomFeldman", customFeldman);
    imshow("CustomScharr", customScharr);
    waitKey(0);
}




