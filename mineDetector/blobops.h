#ifndef BLOBOPS_H
#define BLOBOPS_H

#include "typedefs.h"

using namespace cv;

class BlobOps{
public:

    static void addBlob2Image(KeyPoint &kp, Mat &image, ushort color=0){
        int rad = int(kp.size);                                       //Update thumbnail image with spots left
        int xpos = int(kp.pt.x), ypos = int(kp.pt.y);

        int scalef = 1; //... uh...
        int ypos_sf = ypos/scalef, xpos_sf = xpos/scalef;

        // color:   0 --> white
        //          1 --> red
        //          2 --> green
        //          3 --> yellow


        for (int i=-rad; i < rad; i++){
            for (int j=-rad; j < rad; j++){
                Vec3b &pixval2 = image.at<Vec3b>((ypos_sf)+j,(xpos_sf)+i);  //Default Green = good
                pixval2.val[2] = pixval2.val[1] = pixval2.val[0] = uchar(0); //Black

                switch(color){
                    case 0: // white
                        pixval2.val[2] = pixval2.val[1] = pixval2.val[0] = uchar(200); //white
                        break;
                    case 1: // red
                        pixval2.val[0] = uchar(200);
                        break;
                    case 2: // green
                        pixval2.val[1] = uchar(200);
                        break;
                    case 3: // yellow
                        pixval2.val[0] = pixval2.val[1] = uchar(200);
                        break;
                }
            }
        }
    }


    static float dist_between(Point2f &p1, Point2f &p2){
        int x_dist = p1.x - p2.x,  y_dist = p1.y - p2.y;
        return sqrt( (x_dist*x_dist)+(y_dist*y_dist) );
    }


    static KeyPoint averageKP(KeyPoint &kp1, KeyPoint &kp2)
    {
        float angle = (kp1.angle + kp2.angle)/2;
        float response = (kp1.response + kp2.response)/2;
        float size = (kp1.size + kp2.size)/2;

        Point2f p1 = kp1.pt, p2 = kp2.pt;
        float av_x = (p1.x + p2.x)/2;
        float av_y = (p1.y + p2.y)/2;

        return KeyPoint(av_x,av_y,size,angle,response);
    }


    static map<float,KeyPoint> sortByProximity(Point2f point, keyvect blobs){
        keymap sorted_sizes; //Sorts blobs on radius key automatically (asc)

        // Sort Blobs from biggest to smallest
        for (int k=0; k<blobs.size(); k++)
        {
            KeyPoint &kp = blobs[k];
            Point2f pp = kp.pt;

            float proxy = dist_between(point,pp);
            sorted_sizes[proxy]=kp;
        }
    }

    /** Returns the closest matching blobs between two blob sets/images **/
    static keymap findSimilarities(keyvect blobset1, keyvect blobset2,
            float dist_thresh)
    {
//        compmap min_dist_map;
        keymap min_dist_map;

        keyvect::iterator it1, it2;

        //Populate map and list
        for(it1=blobset1.begin(); it1!=blobset1.end(); ++it1){
            for(it2=blobset2.begin(); it2!=blobset2.end(); ++it2)
            {
                KeyPoint &kp1 = (*it1), &kp2 = (*it2);

                float dist_is = dist_between(kp1.pt,kp2.pt);
                if (dist_is < dist_thresh){
//                    min_dist_map[dist_is] = {kp1,kp2};
                    KeyPoint av_kp = averageKP(kp1,kp2);
                    av_kp.size *= 1.2;  // Make rock locus larger for easier targetting
                    min_dist_map[dist_is] = av_kp;
                }
            }
        }
        //Map is already sorted in asc,
        return min_dist_map;
    }
};

#endif
