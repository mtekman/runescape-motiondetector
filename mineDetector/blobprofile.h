#ifndef BLOBPROF_H
#define BLOBPROF_H

#include "params.h"

class BlobProfile{
public:
    //Populate with blobs
    keyvect keypoints;

    BlobProfile(Mat &fgmask, bool findrock, Mat *search_mask=0)
    {
        // Image is a colour image if looking for rocks,
        // or a binary diff if looking for twinkles
//        Params *pm;
//        if(findrock) pm = new RockParams;
//        else pm = new TwinkleParams;
        Params *pm = new TwinkleParams;

        //Create detector
        Ptr<FeatureDetector> blob_detector = new SimpleBlobDetector(pm->params);
        blob_detector->create("SimpleBlob");

        if (search_mask==0) blob_detector->detect(fgmask, keypoints);
        else blob_detector->detect(fgmask, keypoints, *search_mask);
    }
};

#endif
