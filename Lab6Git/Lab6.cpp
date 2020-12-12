
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>


#include <iostream>
#include <string.h>

#include <cmath>

using namespace std;
using namespace cv;

int findFactors(vector<double>&, vector<double>&, vector<double>&);






int main()
{
    Mat frame;

    VideoCapture _source_("source.mp4");
    int widthTop = 69, HeigthTop = 74, widthBot = 95, HeigthBot = 91;
    vector<double> lFact, rFact;
    while (1) {

        _source_ >> frame;
        float rows = frame.rows;
        float cols = frame.cols;


        float x1 = cols / 100.0 * widthBot;
        float x2 = cols / 100.0 * widthTop;
        float x3 = cols / 100.0 * (100.0 - widthTop);
        float x4 = cols / 100.0 * (100.0 - widthBot);
        float y14 = rows / 100.0 * HeigthBot;
        float y23 = rows / 100.0 * HeigthTop;


        vector<Point2f> formerBinaryPoints;

        vector<Point> polyline;
        polyline.push_back(Point(x2, y23));
        polyline.push_back(Point(x3, y23));
        polyline.push_back(Point(x4, y14));
        polyline.push_back(Point(x1, y14));

        vector<Point2f> points2Transform;
        points2Transform.push_back(Point2f(x2, y23));
        points2Transform.push_back(Point2f(x3, y23));
        points2Transform.push_back(Point2f(x4, y14));
        points2Transform.push_back(Point2f(x1, y14));


        vector<Point2f> res;
        res.push_back(Point2f(240, 0.0));
        res.push_back(Point2f(0.0, 0.0));
        res.push_back(Point2f(0.0, 240));
        res.push_back(Point2f(240, 240));

        Mat resFrameEx = frame.clone();        
        Mat resFrameCur = resFrameEx.clone();
        polylines(resFrameEx, polyline, 1, Scalar(255, 0, 0), 4);
        Mat trasformMatr = getPerspectiveTransform(points2Transform, res);
        Mat resMat;
        warpPerspective(frame, resMat, trasformMatr, Size(240, 240)); 

        Mat res_Binary = resMat.clone();
        cvtColor(res_Binary, res_Binary, COLOR_BGR2HSV);          //Переводим в HSV (Лучше работает, чем оттенки серого как в прошлой лабе работает)
        inRange(res_Binary, Scalar(0, 0, 180), Scalar(255, 255, 255), res_Binary);   //Светлое ищем

        Mat curRectMat;

        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 11; j++) {
                Rect rect(j * 20, i * 40, 40, 40);
                curRectMat = res_Binary(rect);
                Moments mom = moments(curRectMat, true);
                if (mom.m00 > 100) {
                    double X = j * 20;
                    double Y = i * 40;
                    Point2f point(X + float(mom.m10 / mom.m00), Y + float(mom.m01 / mom.m00));  //Аналогично Lab5
                    bool closePointFlag = 0;                            //Флаг для проверки нормы(расстояния) между точками 
                    for (int n = 0; n < formerBinaryPoints.size(); n++) {   //Проверяем все точки, которые уже на этом фрейме ставим на расстояние до новой
                        if (norm(formerBinaryPoints[n] - point) < 20) {     //Если расстояние слишком маленькое хотя бы до одной из точек
                            closePointFlag = 1;                         //Следующее ветвление не выполнится, переходим к следующей итерации                         
                        }
                    }
                    if (!closePointFlag) {
                        formerBinaryPoints.push_back(point);                                  
                    }
                }
            }
        }

        vector<Point2f> lPoints;                                //Точки слева
        vector<Point2f> rPoints;                                //Точки справа
                
        for (int i = 0; i < formerBinaryPoints.size(); i++) {   //Смотрим массив со всеми точками на Фрейме
            (formerBinaryPoints[i].x > 120) ? rPoints.push_back(Point2f(formerBinaryPoints[i].x, formerBinaryPoints[i].y)) : //Точки справа пишем в массив rPoints
             lPoints.push_back(Point2f(formerBinaryPoints[i].x, formerBinaryPoints[i].y));                  //Точки слева - в массив lPoints
        }

        if (lPoints.size() > 3) {               //Если на Фрейме достаточно много точек слева
            vector <double> xLeftArray;              //Массив со всеми координатами точек слева по X
            vector<double> yLeftArray;              //Массив со всеми координатами точек слева по Y
            for (int i = 0; i < lPoints.size(); i++) {
                xLeftArray.push_back(lPoints[i].x);        //Заполняем эти массивы
                yLeftArray.push_back(lPoints[i].y);
            }
            findFactors(yLeftArray, xLeftArray,lFact);      //Ищем коэффициенты a,b,c для составления уравнения интерполяции параболой x = ay+by^2+c
        }

        vector<Point2f> left_line;                                  //Вектор с координатамии кругов, которые мы будем рисовать на координаты после трансформации перспективы
        for (float i = 0.0; i < res_Binary.rows; i += 0.2) {
            left_line.push_back(Point2f(lFact[0] + lFact[1] * i + lFact[2] * pow(i, 2), i)); //Заполняем массив на будущее
            circle(res_Binary, Point2f(lFact[0] + lFact[1] * i + lFact[2] * pow(i, 2), i), 5, Scalar(128), -1);  //На бинаризированном Bird Eye рисуем круги (
        }
        //Аналогично для правой части
        if (rPoints.size() > 3) {
            vector<double> xRightArray;
            vector<double> yRightArray;
            for (int i = 0; i < rPoints.size(); i++) {
                xRightArray.push_back(rPoints[i].x);
                yRightArray.push_back(rPoints[i].y);
            }
            findFactors(yRightArray, xRightArray,rFact);    
        }

        vector<Point2f> right_line;
        for (float i = 0.0; i < res_Binary.rows; i += 0.2) {
            right_line.push_back(Point2f(rFact[0] + rFact[1] * i + rFact[2] * pow(i, 2), i));
            circle(res_Binary, Point2f(rFact[0] + rFact[1] * i + rFact[2] * pow(i, 2), i), 5, Scalar(128), -1);
        }

        if (left_line.size() > 0) {                         //Если левая полоса есть
            vector<Point2f> leftDotArray;                   //Массив с точками этой полосы
            perspectiveTransform(left_line, leftDotArray, trasformMatr.inv());  //Трансформируем по матрице перехода аналогично Lab5
            for (int i = 0; i < leftDotArray.size(); i++) {
                circle(resFrameCur, leftDotArray[i], 3, Scalar(0, 0, 0), -1); //Сосбтвенно рисуем круги
            }
        }
        //Аналогично для правой части
        if (right_line.size() > 0) {
            vector<Point2f> rightDotArray;
            perspectiveTransform(right_line, rightDotArray, trasformMatr.inv());
            for (int i = 0; i < rightDotArray.size(); i++) {
                circle(resFrameCur, rightDotArray[i], 3, Scalar(0, 0, 0), -1);
            }

        }

        imshow("Frame with lines", resFrameCur);
        imshow("Bird", resMat);
        imshow("Binary", res_Binary);


        char c = (char)waitKey(25);
        if (c == 27)
            break;
    }

    destroyAllWindows();

    return 0;
}




int findFactors(vector<double>& x, vector<double>& y,  vector<double>& coeffs){
    int N = x.size();
    double tmp;
    std::vector<double> X(5);
    for (int i = 0; i < 5; ++i) {
        X[i] = 0;
        for (int j = 0; j < N; ++j)
            X[i] += pow(x[j], i);
    }
    std::vector<double> a(3); 
    std::vector<std::vector<double> > B(3, std::vector<double>(4, 0));

    for (int i = 0; i <= 2; ++i)
        for (int j = 0; j <= 2; ++j)
            B[i][j] = X[i + j];

    std::vector<double> Y(3);
    for (int i = 0; i < 3; ++i) {
        Y[i] = (double)0;
        for (int j = 0; j < N; ++j) {
            Y[i] += pow(x[j], i) * y[j];
        }
    }
    for (int i = 0; i <= 2; ++i)
        B[i][3] = Y[i];


    int nm1 = 2;

    for (int i = 0; i < 3; ++i)
        for (int k = i + 1; k < 3; ++k)
            if (B[i][i] < B[k][i])
                for (int j = 0; j <= 3; ++j) {
                    tmp = B[i][j];
                    B[i][j] = B[k][j];
                    B[k][j] = tmp;
                }

    for (int i = 0; i < nm1; ++i)
        for (int k = i + 1; k < 3; ++k) {
            double t = B[k][i] / B[i][i];
            for (int j = 0; j <= 3; ++j)
                B[k][j] -= t * B[i][j];         
        }

    for (int i = nm1; i >= 0; --i) {
        a[i] = B[i][3];                   
        for (int j = 0; j < 3; ++j)
            if (j != i)
                a[i] -= B[i][j] * a[j];       
        a[i] /= B[i][i];                  
    }

    coeffs.resize(a.size());
    for (size_t i = 0; i < a.size(); ++i)
        coeffs[i] = a[i];

    return true;
}