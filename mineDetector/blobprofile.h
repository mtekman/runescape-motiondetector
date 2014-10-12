#ifndef BLOBPROF_H
#define BLOBPROF_H

#include "params.h"

class BlobProfile{
   //*blob_detector;
    Params *pm;

public:
    //Populate with blobs
    keyvect keypoints;


    BlobProfile(Mat &fgmask)
    {
        // Image is a colour image if looking for rocks,
        // or a binary diff if looking for twinkles
//        Params *pm;
//        if(findrock) pm = new RockParams;
//        else pm = new TwinkleParams;
        pm = new TwinkleParams();

        //Create detector
        // Ptr is a ref counter, once it reaches zero, deletes itself
        Ptr<FeatureDetector> blob_detector = new SimpleBlobDetector(pm->params);
        blob_detector->create("SimpleBlob");
        blob_detector->detect(fgmask, keypoints);
    }

    ~BlobProfile(){
//        delete blob_detector;
        delete pm;
    }
};

#endif
