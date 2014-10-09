#ifndef OREFINDER_H
#define OREFINDER_H

#include "blobprofile.h"
#include "cvfuncs.h"
#include "blobops.h"
#include "diffimage.h"


#define DEBUG_KP(kpma)\
    for(keyvect::iterator kit= kpma.begin(); kit!= kpma.end(); ++kit){\
        KeyPoint kp = *kit;\
        cerr << " ([" << kp.pt.x << "," << kp.pt.y << "] " << kp.size << ")" << flush;\
    }\



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

        if (debug){
            cerr << "Early:" << flush; DEBUG_KP(bp1->keypoints); cerr << endl;
            cerr << "Later:" << flush; DEBUG_KP(bp2->keypoints); cerr << endl;
        }

        //Diff images
        DiffImage *di = new DiffImage(early,later);


        //Find Twinkles within the diffimage, specifically those in the rock point locations
        // Make a search_mask
        Mat *search_mask = new Mat(Mat::zeros(early.rows, early.cols, CV_8U));
        CVFuncs::addBlobMap2Image(rock_regions, *search_mask);

        //Now find twinkle points
        BlobProfile *twink = new BlobProfile(di->fgmask, false, search_mask);
        ore_locs = twink->keypoints;


        if(debug){
            Mat debug_img(early);

            float scalef = 1.5;

            Mat search_mask_img;
            Mat search_three_channel [] = {*search_mask, *search_mask, *search_mask};
            merge(search_three_channel, 3, search_mask_img);

            search_mask_img *= 5;
            debug_img += search_mask_img;

            resize(debug_img, debug_img, Size(early.cols/scalef, early.rows/scalef));


            imshow("Debug", *search_mask);
            moveWindow("Debug", 960, 0);
            waitKey(0);
        }
    }
};

#endif
