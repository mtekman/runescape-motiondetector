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
        // POSTERIZE
        early /=100; early *=100;
        later /=100; later *=100;

        fgmask = later - early  ;      //Simple subtraction
    }

};

#endif
