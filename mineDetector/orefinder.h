#ifndef OREFINDER_H
#define OREFINDER_H

#include "blobprofile.h"
#include "blobops.h"
#include "diffimage.h"


/** finds rocks between two images and finds twinkle points **/
struct OreFinder{
    keyvect ore_locs;

    OreFinder(Mat &early, Mat &later, bool debug=false)
    {
        //Populate keypoints of rocks for both images
        BlobProfile *bp1 = new BlobProfile(early, true);
        BlobProfile *bp2 = new BlobProfile(later, true);

        //Find common Rock points in general
        keymap rock_regions = BlobOps::findSimilarities(bp1->keypoints, bp2->keypoints, 10);

        //Diff images
        DiffImage *di = new DiffImage(early,later);

        //Find Twinkles within the diffimage, specifically those in the rock point locations
        // Make a search_mask
        Mat *search_mask = new Mat(Mat::zeros(early.rows, early.cols, CV_8U));

        for (keymap::const_iterator itt = rock_regions.begin(); itt != rock_regions.end(); ++itt){
            float dist = itt->first;
            KeyPoint ka = itt->second;
            BlobOps::addBlob2Image(ka, *search_mask);
        }

        //Now find twinkle points
        BlobProfile *twink = new BlobProfile(di->fgmask, false, search_mask);
        ore_locs = twink->keypoints;
    }
};

#endif
