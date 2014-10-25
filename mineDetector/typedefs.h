#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

typedef vector<cv::KeyPoint> keyvect;
typedef map<float,KeyPoint> keymap;

#define DEBUGIMG false
//#define DEBUGIMG true


#define HULK_X 200
#define HULK_Y 10

#define showIMG(img, title)\
    if(DEBUGIMG) imshow(title, img); moveWindow(title,HULK_X,HULK_Y);waitKey(0)

#define showHSV(img, title)\
    if (DEBUGIMG){Mat bgr; cvtColor(img,bgr,CV_HSV2BGR);\
    imshow(title, bgr); moveWindow(title,HULK_X,HULK_Y);waitKey(0);}



#endif
