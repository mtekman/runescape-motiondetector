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

//        filterPreviousBlacks(early, later, twink.keypoints, debug);

        if (debug){
            Mat debbie = early.clone()/2 + later.clone()/2;
            debbie /= 3;

            showIMG(debbie);

            Mat debbie2;
            vector<Mat> vec;
            vec.push_back(di.fgmask); vec.push_back(di.fgmask); vec.push_back(di.fgmask);
            merge(vec,debbie2);

            debbie2 /= 1.5;

            Mat king = debbie + debbie2;
            showIMG(king);

            CVFuncs::addBlobVect2Image(twink.keypoints, king);
            showIMG(king);
            showIMG(di.fgmask);
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
        float limit = 500;

        for (keyvect::iterator it= keypoints.begin();it != keypoints.end();)
        {
            KeyPoint &kp = *it;
            Point pt = kp.pt;

            if (debug){
                cerr << endl;
                cerr << pt.x << "--" << pt.y << endl;
            }

            bool good2go=false;

            float one_pixel_set_b =0, one_pixel_set_g =0, one_pixel_set_r =0;
            float two_pixel_set_b =0, two_pixel_set_g =0, two_pixel_set_r =0;

            for (int c=-range; c< range ;c += step){
                for (int s= -range; s < range; s += step){
                    Vec3b pixel_set1 = early.at<Vec3b>(pt.x + c, pt.y +s);
                    Vec3b pixel_set2 = later.at<Vec3b>(pt.x + c, pt.y +s);

                    uchar &b1 = pixel_set1[0], &g1 = pixel_set1[1], &r1 = pixel_set1[2];
                    uchar &b2 = pixel_set2[0], &g2 = pixel_set2[1], &r2 = pixel_set2[2];

                    one_pixel_set_b += b1; one_pixel_set_g += g1; one_pixel_set_r += r1;
                    two_pixel_set_b += b2; two_pixel_set_g += g2; two_pixel_set_r += r2;

                }
            }

            float b_on_g_1 = (one_pixel_set_b/one_pixel_set_g), g_on_r_1 = (one_pixel_set_g/one_pixel_set_r);
            float b_on_g_2 = (two_pixel_set_b/two_pixel_set_g), g_on_r_2 = (two_pixel_set_g/two_pixel_set_r);

            float bgr1 = (b_on_g_1/g_on_r_1), bgr2 = (b_on_g_2/g_on_r_2);


            if (    (one_pixel_set_b > 2)
                    &&
                    ((bgr1 > 0.6 && bgr1 < 1.1 && one_pixel_set_b < limit)
                    ||
                    (bgr2 > 0.6 && bgr2 < 1.1 && two_pixel_set_b < limit))) good2go = true;

            if (debug){
                cerr << "[" << pt.x << "," << pt.y << "] = " << flush;
                cerr << b_on_g_1 << " " << g_on_r_1 << " " << bgr1 << " " << one_pixel_set_b << ", " << flush;
                cerr << b_on_g_2 << " " << g_on_r_2 << " " << bgr2 << " " << two_pixel_set_b << " == " << good2go << endl;
            }

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
