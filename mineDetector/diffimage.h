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

        int dilation_size = 4;

        dilate(fgmask, fgmask, getStructuringElement(MORPH_ELLIPSE,
                                                     Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                                     Point( dilation_size, dilation_size ) ));

        // Filter out non-whites
        Mat splitter[3];
        split(fgmask,splitter);

        Mat blue = splitter[0] > 100, green = splitter[1] > 100, red = splitter[2] > 100;
        Mat blue_on_green = blue/green;
        Mat green_on_red = green/red;

        Mat whiter = (blue_on_green/green_on_red);
        fgmask = whiter * 230;

    }
};

#endif
