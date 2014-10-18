#ifndef PARAMS_H
#define PARAMS_H

#include "typedefs.h"

struct Params{
    SimpleBlobDetector::Params params;

    Params(){
        params.filterByInertia = false;
        params.filterByConvexity = false;
        params.filterByArea = true;
    }
};

struct TwinkleParams : Params{
    TwinkleParams(){
        //Difference image, no colour
        params.minDistBetweenBlobs = 0.00001f;
        params.filterByCircularity = false;

//        params.filterByColor = true;
//        params.blobColor = 200;

        // Twinkle size
        params.minArea = 1.0f;
        params.maxArea = 1000.0f;
    }
};
#endif
