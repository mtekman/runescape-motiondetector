#ifndef OREFINDER_H
#define OREFINDER_H

#include "blobprofile.h"
#include "cvfuncs.h"
#include "blobops.h"
#include "diffimage.h"

#include "rockfinder.h"

/** finds rocks between two images and finds twinkle points **/
struct OreFinder{
    keyvect ore_locs;


    OreFinder(Mat &early, Mat &later, bool debug=false)
    {
        //Populate keypoints of rocks for both images

//        RockFinder *rf = new RockFinder(early);
 //       exit(0);


/*      BlobProfile *bp1 = new BlobProfile(early, true);
        BlobProfile *bp2 = new BlobProfile(later, true);

        //Find common Rock points in general
        keymap rock_regions = BlobOps::findSimilarities(bp1->keypoints, bp2->keypoints, 10);

        if (debug){
            CVFuncs::showKeyVectIMG(early,bp1->keypoints); // rock_regions);
            CVFuncs::showKeyVectIMG(later,bp2->keypoints); //rock_regions);
            CVFuncs::showKeyMapIMG(early,rock_regions);
        }
*/

        //Diff images
        DiffImage *di = new DiffImage(early,later);
        BlobProfile *twink = new BlobProfile(di->fgmask, false);

        if (debug){
            early /= 4;
            CVFuncs::addBlobVect2Image(twink->keypoints, early);
            showIMG(early, 1200, 0);

            showIMG(di->fgmask, 1200, 0 );
        }

        ore_locs = twink->keypoints;
    }
};

#endif
