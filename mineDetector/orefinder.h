#ifndef OREFINDER_H
#define OREFINDER_H

#include "blobprofile.h"
#include "cvfuncs.h"
#include "blobops.h"
#include "diffimage.h"

/** finds rocks between two images and finds twinkle points **/
struct OreFinder{
    keyvect ore_locs;

    OreFinder(Mat &early, Mat &later, bool debug=false)
    {
        //Diff images
        DiffImage di(early,later);
        BlobProfile twink(di.fgmask);

        filterPreviousBlacks(early, later, twink.keypoints, debug);

        if (debug){
            Mat debbie = early.clone()/2 + later.clone()/2;
            debbie /= 3;

            showIMG(debbie, 1000,0);

            Mat debbie2;
            vector<Mat> vec;
            vec.push_back(di.fgmask); vec.push_back(di.fgmask); vec.push_back(di.fgmask);
            merge(vec,debbie2);

            debbie2 /= 1.5;

            Mat king = debbie + debbie2;
            showIMG(king, 1000, 0);

            CVFuncs::addBlobVect2Image(twink.keypoints, king);
            showIMG(king, 1000, 0);
            showIMG(di.fgmask,1000,0);
        }
        ore_locs = twink.keypoints;
    }


    /** Checks keypoint locations to see if they
     * correspond to spots that were previously
     * black (coal deposits). **/
    void filterPreviousBlacks(
            Mat &early, Mat &later,
            keyvect &keypoints, bool &debug)
    {
        int range = 5;
        int step = 3;
        float limit = 100;

        for (keyvect::iterator it= keypoints.begin();it != keypoints.end();)
        {
            KeyPoint &kp = *it;
            Point pt = kp.pt;

            if (debug){
                cerr << endl;
                cerr << pt.x << "--" << pt.y << endl;
            }

            Point min;
            float min_sq= 999999999999;
//            Vec3b chosen_set [2];
            bool good2go=false;

            for (int c=-range; c< range ;c += step){
                for (int s= -range; s < range; s += step){
                    Vec3b pixel_set1 = early.at<Vec3b>(pt.x + c, pt.y +s);
                    Vec3b pixel_set2 = later.at<Vec3b>(pt.x + c, pt.y +s);

                    uchar &b1 = pixel_set1[0], &g1 = pixel_set1[1], &r1 = pixel_set1[2];
                    uchar &b2 = pixel_set2[0], &g2 = pixel_set2[1], &r2 = pixel_set2[2];

                    //                    float pic_diff = (float)(b1+g1+r1)/(b2+g2+r2);
                    //                    if (pic_diff < (1/limit) || pic_diff > limit){

                    if (b1 < limit || g1 < limit || r1 < limit
                            || b2 < limit || g2 < limit || r2 < limit){

/*                        float square = c*c + s*s;
                        if (square < min_sq){
                            min_sq = square;
                            min.x = c;
                            min.y = s;
                            chosen_set[0] = pixel_set1;
                            chosen_set[1] = pixel_set2;
                        }*/
                        good2go = true;
                    }
                }
            }
//            kp.pt.x += min.x;
//            kp.pt.y += min.y;

            if (debug){
                cerr << "[" << kp.pt.x << "," << kp.pt.y << "] = " << flush;
//                cerr << (int)chosen_set[0][0] << " " << (int)chosen_set[0][1] << " " << (int)chosen_set[0][2] << "," << flush;
//                cerr << (int)chosen_set[1][0] << " " << (int)chosen_set[1][1] << " " << (int)chosen_set[1][2] << endl;
            }

            //If neither are within lim, delete
//            if (chosen_set[0] == Vec3b({0,0,0}))
            if (!good2go)
            {
                it = keypoints.erase(it); //give next iterator
            }
            else ++it;
        }


        if (debug){
            cerr << "\nFinal List:" << endl;
            for (keyvect::iterator it= keypoints.begin();it != keypoints.end(); ++it){
                cerr << (*it).pt.x << " - " << (*it).pt.y << endl;
            }
        }
    }
};

#endif
