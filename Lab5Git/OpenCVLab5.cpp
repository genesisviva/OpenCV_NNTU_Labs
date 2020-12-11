#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


#include <iostream>
#include <string.h>

using namespace std;
using namespace cv;


int main()
{
    Mat frame;
   
    VideoCapture source("source.mp4");
    namedWindow("Params");
    int widthTop = 69, HeigthTop = 74, widthBot = 95, HeigthBot = 91;
    createTrackbar("Width up plane", "Params", &widthTop, 100);
    createTrackbar("Heigth up plane", "Params", &HeigthTop, 100);
    createTrackbar("Width low plane", "Params", &widthBot, 100);
    createTrackbar("Heigth low plane", "Params", &HeigthBot, 100);

    namedWindow("BirdEye", WINDOW_AUTOSIZE);
    vector<Point2f> Binary;
    while (1) {

        source >> frame;
        float rows = frame.rows;
        float cols = frame.cols;

        float x1 = cols / 100.0 * widthBot;
        float x2 = cols / 100.0 * widthTop;
        float x3 = cols / 100.0 * (100.0 - widthTop);
        float x4 = cols / 100.0 * (100.0 - widthBot);
        float y14 = rows / 100.0 * HeigthBot;
        float y23 = rows / 100.0 * HeigthTop;

        Binary.clear();

        vector<Point> polyline;
        polyline.push_back(Point(x1, y14));
        polyline.push_back(Point(x2, y23));
        polyline.push_back(Point(x3, y23));
        polyline.push_back(Point(x4, y14));

        vector<Point2f> points2Transform;
        points2Transform.push_back(Point2f(x1, y14));
        points2Transform.push_back(Point2f(x2, y23));
        points2Transform.push_back(Point2f(x3, y23));
        points2Transform.push_back(Point2f(x4, y14));

       vector<Point2f> res;
        res.push_back(Point2f(240, 0.0));
        res.push_back(Point2f(0.0, 0.0));
        res.push_back(Point2f(0.0, 240));
        res.push_back(Point2f(240, 240));


        Mat resFrame = frame.clone();         
        polylines(resFrame, polyline, 1, Scalar(0, 255, 0), 4);
        Mat trasformMatr = getPerspectiveTransform(points2Transform, res); //Матрицу перехода создаем
        Mat resMat;
        warpPerspective(frame, resMat, trasformMatr, Size(240, 240));

        Mat curRectMat;                                     //Матрица для хранения матрицы, полученной из прямоугольника
        Mat resG_Binary = resMat.clone();  
        
        cvtColor(resG_Binary, resG_Binary, COLOR_BGR2GRAY); //Конвертация в серый
        inRange(resG_Binary, Scalar(170, 170, 170), Scalar(255, 255, 255), resG_Binary);   //Ищем белый(серый) цвет
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 5; j++) {                   //Разбиваем матрицу на прямоугольники
                Rect rect(j * 40, i * 80, 80, 80);          //Конкретный прямоугольник
                curRectMat = resG_Binary(rect);                 //Выделенную область преобразуем в матрицу
                Moments mom = moments(curRectMat, true);    //Рассчитываем моменты.(true - т.к. бинарное изображение)
                if (mom.m00 > 350) {                        //Если интенсивность большая.          
                    double X = (mom.m10 / mom.m00);         //Центр участка (по X) (для фигуры в прямоугольнике)
                    double Y = (mom.m01 / mom.m00);         //Центр участка (по X) (для фигуры в прямоугольнике)
                    Point2f dot(j * 40 + X, i * 80 + Y);   //Центр конкретного участка, но с координатной сеткой resGray
                    Binary.push_back(dot);                  //Для отображения на основном фрейме записываем в массив
                    circle(resG_Binary, dot, 3, Scalar(0), -1); //Рисуем круг
                }
            }
        }

        if (Binary.size() > 0) {                              //Если есть точка, чтобы отобразить
            vector<Point2f> dotArray;
            perspectiveTransform(Binary, dotArray, trasformMatr.inv());     //Записываем в dotArray значения Binary (перобразованные для координат resFrame, используя перспективу)
            for (size_t i = 0; i < Binary.size(); i++) {                    
                circle(resFrame, dotArray[i], 3, Scalar(0, 0, 0), -1);      //Рисуем круги на основном фрейме
            }
        }
        imshow("Frame", resFrame);
        imshow("Bird", resMat);
        imshow("Binary", resG_Binary);


        char c = (char)waitKey(25);
        if (c == 27)
            break;
    }

    destroyAllWindows();

    return 0;
}