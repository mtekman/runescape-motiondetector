#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

typedef vector<cv::KeyPoint> keyvect;
typedef map<float,KeyPoint> keymap;
//typedef map<float,KeyPoint[2]> compmap;

#define showIMG(img, x,y)\
    imshow("img", img); moveWindow("img",x,y);waitKey(0)


#endif
