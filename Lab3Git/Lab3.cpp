#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main()
{
    VideoCapture vid(0);
    while (1) {
        Mat frame, frameBlur, frameHSV, frameYellow, frameCanny;
        vid >> frame;
        
        blur(frame, frameBlur, Size (8, 8));

        cvtColor(frame, frameHSV, COLOR_BGR2HSV);
        inRange(frameHSV, Scalar(20, 100, 100), Scalar(30, 255, 255), frameYellow);
        
        Canny(frame, frameCanny, 100, 100);
       

        

        if (frame.empty())
            break;
        imshow("Original", frame);
        imshow("Blured", frameBlur);
        imshow("Yellow", frameYellow);
        imshow("Canny", frameCanny);
        char c = (char)waitKey(25);
        if (c == 27)
            break;
    }
    vid.release();
    destroyAllWindows();

    return 0;
}

