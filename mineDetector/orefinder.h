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
        DiffImage di(early,later);
        BlobProfile twink(di.fgmask);

//        if (debug){
//            debbie /= 4;
//            CVFuncs::addBlobVect2Image(twink.keypoints, debbie);
//            showIMG(debbie, 1200, 0);
//        }

        filterPreviousBlacks(early, later, twink.keypoints);

//        if (debug){
//            showIMG(debbie, 1200, 0);
//        }

        ore_locs = twink.keypoints;
    }

    /** Checks keypoint locations to see if they
     * correspond to spots that were previously
     * black (coal deposits). **/
    void filterPreviousBlacks(Mat &early, Mat &later,
            keyvect &keypoints)
    {
        for (keyvect::iterator it= keypoints.begin();it != keypoints.end(); ++it)
        {
            KeyPoint &kp = *it;
            Point pt = kp.pt;

            Vec3b average_set1;
            Vec3b average_set2;

            char range = 150;
            uchar limit = 150;


            for (char c=-range; c< range ;c++){
                for (char s= -range; s < range; s++){
                    Vec3b pixel_set1 = early.at<Vec3b>(pt.x + c, pt.y +s);
                    Vec3b pixel_set2 = later.at<Vec3b>(pt.x + c, pt.y +s);

//                    uchar &b1 = pixel_set1[0], &g1 = pixel_set1[1], &r1 = pixel_set1[2];
//                    uchar &b2 = pixel_set2[0], &g2 = pixel_set2[1], &r2 = pixel_set2[2];

                    cerr << "YODELAI" << endl;
                    if (pixel_set1 != pixel_set2){

//                        if (b1 < limit || g1 < limit || r1 < limit || b2 < limit || g2 < limit || r2 < limit){
//                            cerr << "YOOO " << (int)c << "," << (int)s << endl;
//                            cerr << (int)pixel_set1[0] << "-" << (int)pixel_set1[1] << "-" << (int)pixel_set1[2] << "," << flush;
//                            cerr << (int)pixel_set2[0] << "-" << (int)pixel_set2[1] << "-" << (int)pixel_set2[2] << endl;
//                        }
                    }
                    average_set1 += pixel_set1;
                    average_set2 += pixel_set2;
                }
            }
            average_set1 /= range * range;
            average_set2 /= range * range;


//            //If neither are within lim, delete
//            if (!((b1 < limit && g1 < limit && r1 < limit)
//                ||
//                (b2 < limit && g2 < limit && r2 < limit)))
//            {
//                it = keypoints.erase(it); //give next iterator
//            }
//            ++it;
        }

    }


};

#endif
