#ifndef DIFFIMAGE_H
#define DIFFIMAGE_H

#include "typedefs.h"

class DiffImage
{
public:
    Mat fgmask;
    bool empty;

    DiffImage(Mat &early, Mat &later){
        empty=false;

        difference(early,later);

        if (notEmpty()){}
        else empty=true;
    }

private:
    bool notEmpty(){
        Scalar sums = sum(fgmask);
        if ((sums.val[0] > 10 ) || (sums.val[1] > 10) || (sums.val[2]>10) ) return true;
        return false;
    }


    void difference(Mat &early,Mat &later)
    {
        Mat fgmask1 = later - early;      //Simple subtraction
        Mat fgmask2 = early - later;

        fgmask = fgmask1 + fgmask2;

        fgmask /= 50;
        fgmask *= 50;

        int erosion_size = 4;
        Mat element = getStructuringElement(MORPH_ELLIPSE,
                                            Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                            Point( erosion_size, erosion_size ) );

        dilate(fgmask, fgmask, element);

        Mat splitter[3];
        split(fgmask,splitter);

        Mat blue = splitter[0] > 100, green = splitter[1] > 100, red = splitter[2] > 100;
        Mat blue_on_green = blue/green;
        Mat green_on_red = green/red;

        Mat whiter = (blue_on_green/green_on_red);
        whiter *=230;

//        float thresh = 2;
//        whiter = ((whiter < thresh) & (whiter> 1/thresh)) * 200;

        vector<Mat> vec;
        vec.push_back(whiter); vec.push_back(whiter); vec.push_back(whiter);
        merge(vec,fgmask);
    }

};

#endif
