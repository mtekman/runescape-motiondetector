#ifndef BLOBPROF_H
#define BLOBPROF_H

#include "typedefs.h"

struct Params{
    SimpleBlobDetector::Params params;

    Params(){
        params.filterByInertia = false;
        params.filterByConvexity = false;
        params.filterByColor = false;
        params.filterByArea = true;
    }
};

struct TwinkleParams : Params{
    TwinkleParams(){
        //Difference image, no colour
        params.minDistBetweenBlobs = 10.f;
        params.filterByCircularity = false;

//        params.blobColor = 200;

        // Twinkle size
        params.minArea = 30.f;
        params.maxArea = 100.0f;
    }
};


class BlobProfile{
    Params *pm;

public:
    //Populate with blobs
    keyvect keypoints;


    BlobProfile(Mat &fgmask)
    {
        pm = new TwinkleParams();

        //Create detector
        // Ptr is a ref counter, once it reaches zero, deletes itself
        Ptr<FeatureDetector> blob_detector = new SimpleBlobDetector(pm->params);
        blob_detector->create("SimpleBlob");
        blob_detector->detect(fgmask, keypoints);
    }

    ~BlobProfile(){
       delete pm;
    }
};

#endif
