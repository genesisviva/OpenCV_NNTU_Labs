#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int xGradient(Mat image, int x, int y)
{
    return image.at<uchar>(y - 1, x - 1) +
        2 * image.at<uchar>(y, x - 1) +
        image.at<uchar>(y + 1, x - 1) -
        image.at<uchar>(y - 1, x + 1) -
        2 * image.at<uchar>(y, x + 1) -
        image.at<uchar>(y + 1, x + 1);
}




void SobelTransformFilter(const Mat& initial, Mat& result) {
    result = initial.clone();
    int gx;
    for (int y = 0; y < initial.rows; y++)
        for (int x = 0; x < initial.cols; x++)
            result.at<uchar>(y, x) = 0.0;
    for (int y = 1; y < initial.rows - 1; y++) {
        for (int x = 1; x < initial.cols - 1; x++) {
            gx = xGradient(initial, x, y);
            result.at<uchar>(y, x) = gx;
        }
    }
}

void customFilter3Colored(const Mat& initial, Mat& result, vector<double> kernel) {
    result = initial;
    for (int i = 1; i < result.rows - 1; i++) {
        for (int j = 1; j < result.cols - 1; j++) {
            result.at<Vec3b>(i, j)[0] = initial.at<Vec3b>(i - 1, j - 1)[0] * kernel[0] + initial.at<Vec3b>(i, j - 1)[0] * kernel[1] + initial.at<Vec3b>(i + 1, j - 1)[0] * kernel[2] + initial.at<Vec3b>(i - 1, j)[0] * kernel[3] + initial.at<Vec3b>(i, j)[0] * kernel[4] + initial.at<Vec3b>(i + 1, j)[0] * kernel[5] + initial.at<Vec3b>(i - 1, j + 1)[0] * kernel[6] + initial.at<Vec3b>(i, j + 1)[0] * kernel[7] + initial.at<Vec3b>(i + 1, j + 1)[0] * kernel[8];
            result.at<Vec3b>(i, j)[1] = initial.at<Vec3b>(i - 1, j - 1)[1] * kernel[0] + initial.at<Vec3b>(i, j - 1)[1] * kernel[1] + initial.at<Vec3b>(i + 1, j - 1)[1] * kernel[2] + initial.at<Vec3b>(i - 1, j)[1] * kernel[3] + initial.at<Vec3b>(i, j)[1] * kernel[4] + initial.at<Vec3b>(i + 1, j)[1] * kernel[5] + initial.at<Vec3b>(i - 1, j + 1)[1] * kernel[6] + initial.at<Vec3b>(i, j + 1)[1] * kernel[7] + initial.at<Vec3b>(i + 1, j + 1)[1] * kernel[8];
            result.at<Vec3b>(i, j)[2] = initial.at<Vec3b>(i - 1, j - 1)[2] * kernel[0] + initial.at<Vec3b>(i, j - 1)[2] * kernel[1] + initial.at<Vec3b>(i + 1, j - 1)[2] * kernel[2] + initial.at<Vec3b>(i - 1, j)[2] * kernel[3] + initial.at<Vec3b>(i, j)[2] * kernel[4] + initial.at<Vec3b>(i + 1, j)[2] * kernel[5] + initial.at<Vec3b>(i - 1, j + 1)[2] * kernel[6] + initial.at<Vec3b>(i, j + 1)[2] * kernel[7] + initial.at<Vec3b>(i + 1, j + 1)[2] * kernel[8];
        }
    }
}

void customBlurFunc(const Mat& source, Mat& result) {
    vector<double> kernel = { 0.1,0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1 };
    customFilter3Colored(source, result, kernel);
}

int main()
{
    Mat original;
    original = imread("shakal.jpg", cv::IMREAD_COLOR);
    imshow("Original", original);

    Mat blurAPI;
    GaussianBlur(original, blurAPI, Size(7, 47), 0, 0);
    imshow("Blured", blurAPI);

    Mat customBlur;
    customBlurFunc(original, customBlur);
    imshow("CustomBlured", customBlur);


    Mat customSobel;
    Mat src = imread("shakal.jpg", cv::IMREAD_GRAYSCALE);
    SobelTransformFilter(src, customSobel);
    imshow("CustomSobel", customSobel);

    waitKey(0);
}




