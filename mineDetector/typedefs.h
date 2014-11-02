#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

typedef vector<cv::KeyPoint> keyvect;
typedef map<float,KeyPoint> keymap;


#define DEBUGIMG false

// Screen below
//#define HULK_Y 1100
//#define HULK_X 600

#define HULK_Y 10
#define HULK_X 1200


#define gray2HSV(gray, hsv)\
    Mat zz = Mat::ones(int(gray.rows), int(gray.cols), CV_8U);\
    merge({zz,zz,gray},hsv)

#define showIMG(img, title)\
    if(DEBUGIMG) imshow(title, img); moveWindow(title,HULK_X,HULK_Y);waitKey(0)

#define showHSV(img, title)\
    if (DEBUGIMG){Mat bgr; cvtColor(img,bgr,CV_HSV2BGR);\
    imshow(title, bgr); moveWindow(title,HULK_X,HULK_Y);waitKey(0);}



#endif
