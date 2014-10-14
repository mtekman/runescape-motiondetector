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


    void difference(Mat &early,Mat &later){
        // POSTERIZE, no we need all the sparkle we can get
        // plus there's very little noise in a CG image
//        early /=100; early *=100;
//        later /=100; later *=100;

        Mat fgmask1 = later - early;      //Simple subtraction
        Mat fgmask2 = early - later;

        fgmask = fgmask1 + fgmask2;

        int lower=130;
        inRange(fgmask, Scalar(lower, lower, lower), Scalar(255, 255, 255), fgmask);

        int erosion_size = 2;
        Mat element = getStructuringElement(MORPH_CROSS,
                                            Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                            Point( erosion_size, erosion_size ) );
        dilate(fgmask, fgmask, element);


    }

};

#endif
