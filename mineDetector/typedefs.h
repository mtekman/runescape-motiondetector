#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

typedef vector<cv::KeyPoint> keyvect;
typedef map<float,KeyPoint> keymap;
//typedef map<float,KeyPoint[2]> compmap;

#define HULK_X 1000
#define HULK_Y 10

#define showIMG(img)\
    imshow("img", img); moveWindow("img",HULK_X,HULK_Y);waitKey(0)


#define showHSV(img)\
    if (true){Mat bgr; cvtColor(img,bgr,CV_HSV2BGR);\
    imshow("img", bgr); moveWindow("img",HULK_X,HULK_Y);waitKey(0);}


#endif
