#ifndef PARAMS_H
#define PARAMS_H

#include "typedefs.h"

struct Params{
    SimpleBlobDetector::Params params;

    Params(){
        params.filterByInertia = false;
        params.filterByConvexity = false;
        params.filterByColor = true;
        params.filterByArea = true;
    }
};

struct RockParams : Params {
    RockParams(){
        params.minDistBetweenBlobs = 3.0f;

        uchar pixels[3] = {122,101,73}; //RGB, brownish grey
        params.blobColor = *pixels;
        params.filterByCircularity = true;

        // Rock size
        params.minArea = 25.0f;
        params.maxArea = 30.0f;
    }
};


struct TwinkleParams : Params{
    TwinkleParams(){
        //Difference image, no colour
        params.minDistBetweenBlobs = 1.0f;
        params.filterByCircularity = false;

        // Twinkle size
        params.minArea = 3.0f;
        params.maxArea = 5.0f;
    }
};

#endif
