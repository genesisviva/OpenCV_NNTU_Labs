#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;



int main()
{
    Mat frame;
    VideoCapture source("source.mp4");
    namedWindow("Params", WINDOW_AUTOSIZE);
    createTrackbar("Width up plane", "Params", 0, 100);
    createTrackbar("Heigth up plane", "Params", 0, 100);
    createTrackbar("Width low plane", "Params", 0, 100);
    createTrackbar("Heigth low plane", "Params", 0, 100);

    while (1) {
        source >> frame;
        float rows = frame.rows;
        float cols = frame.cols;
        float widthTop = getTrackbarPos("Width up plane", "Params");
        float widthBot = getTrackbarPos("Width low plane", "Params");
        float HeigthTop = getTrackbarPos("Heigth up plane", "Params");
        float HeigthBot = getTrackbarPos("Heigth low plane", "Params");

        float x1 = cols / 100.0 * widthBot;
        float x2 = cols / 100.0 * widthTop;
        float x3 = cols / 100.0 * (100.0 - widthTop);
        float x4 = cols / 100.0 * (100.0 - widthBot);
        float y14 = rows / 100.0 * HeigthBot;
        float y23 = rows / 100.0 * HeigthTop;
        
        vector<Point> polygon;
        polygon.push_back(Point(x1, y14));
        polygon.push_back(Point(x2, y23));
        polygon.push_back(Point(x3, y23));
        polygon.push_back(Point(x4, y14));

        vector<Point2f> points2Trasform;
        points2Trasform.push_back(Point2f(x1, y14));
        points2Trasform.push_back(Point2f(x2, y23));
        points2Trasform.push_back(Point2f(x3, y23));
        points2Trasform.push_back(Point2f(x4, y14));

        vector<Point2f> res;
        res.push_back(Point2f(640.0, 480.0));
        res.push_back(Point2f(640.0, 0.0));
        res.push_back(Point2f(0.0, 0.0));
        res.push_back(Point2f(0.0, 480.0));

        Mat resFrame = frame.clone();
        polylines(resFrame, polygon, 1, Scalar(0, 255, 0), 4);
        Mat trasformMatr = getPerspectiveTransform(points2Trasform, res);
        Mat resMat;
        warpPerspective(frame, resMat, trasformMatr, resMat.size());
        imshow("Frame", resFrame);
        imshow("Bird", resMat);

        char c = (char)waitKey(25);
        if (c == 27)
            break;
    }

  
    destroyAllWindows();
    return 0;
}