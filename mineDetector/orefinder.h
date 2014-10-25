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
        di.fgmask = di.fgmask > 150;
        BlobProfile twink(di.fgmask);

        filterPreviousBlacks(early, later, twink.keypoints, debug);

        if (debug){
            Mat debbie = early.clone()/2 + later.clone()/2;
            debbie /= 3;

            showHSV(debbie, "init");

            Mat debbie2;
            vector<Mat> vec;
            vec.push_back(di.fgmask); vec.push_back(di.fgmask); vec.push_back(di.fgmask);
            merge(vec,debbie2);

            debbie2 /= 1.5;

            Mat king = debbie + debbie2;
            showHSV(king, "king");

            CVFuncs::addBlobVect2Image(twink.keypoints, king);
            showHSV(king, "king+blobs");
            showIMG(di.fgmask, "diff mask");
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
        int step = 1;
        int l_lim = 1000;

        int div = (4*range*range)/(step*step);

        for (keyvect::iterator it= keypoints.begin();it != keypoints.end();)
        {
            KeyPoint &kp = *it;
            Point pt = kp.pt;

            if (debug){
                cerr << endl;
                cerr << pt.x << "--" << pt.y << endl;
            }

            bool good2go=false;

            uint ss1=0,ll1=0;
            uint ss2=0,ll2=0;

            uint count=0;

            for (int c=-range; c< range ;c += step){
                for (int s= -range; s < range; s += step){
                    Vec3b pixel_set1 = early.at<Vec3b>(pt.x + c, pt.y +s);
                    Vec3b pixel_set2 = later.at<Vec3b>(pt.x + c, pt.y +s);

                    uchar &s1 = pixel_set1[1], &l1 = pixel_set1[2];
                    uchar &s2 = pixel_set2[1], &l2 = pixel_set2[2];

                    ss1 += s1; ll1 += l1;
                    ss2 += s2; ll2 += l2;

                    count ++;
                }
            }

            ss1 /= count; ss2 /= count;
            ll1 /= count; ll2 /= count;

            if ( ll1 < l_lim || ll2 < l_lim ) good2go=true;
            if ( ll1 <= 2 && ll2 <= 2 && ss1 <= 2 && ss2 <= 2) good2go=false;


            if (debug){
                cerr << "[" << pt.x << "," << pt.y << "] = " << flush;
                cerr << ss1 << " " << ll1 << ", " << flush;
                cerr << ss2 << " " << ll2 << " " << " == " << good2go << endl;
            }

            if (!good2go) it = keypoints.erase(it); //give next iterator
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
