#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "colortest.h"


static int isqrt(int num) {
    int res = 0, bit = 1 << 30; // The second-to-top bit is set: 1L<<30 for long

    while (bit > num) bit >>= 2;
    while (bit != 0) {
        if (num >= res + bit) {
            num -= res + bit;
            res = (res >> 1) + bit;
        }
        else res >>= 1;
        bit >>= 2;
    }
    return res;
}




class TestEachBlob{
public:
    int proxim;    //Test only if neccesary
    ColorTest *ct;

    TestEachBlob(KeyPoint &kp, Mat &fgmask, int &player_x, int &player_y, int kindex, bool &debug)
    {
        proxim = -1;

        int xpos = int(kp.pt.x);
        int ypos = int(kp.pt.y);

        Vec3b &pix = fgmask.at<Vec3b>(kp.pt.y,kp.pt.x);
        Vec3b neigh = neighPix(xpos, ypos, fgmask);
        if (debug) debugStats(neigh, pix, kindex, xpos, ypos, kp.size);

        ct = new ColorTest(neigh, debug);
        if(ct->pass){
            proxim = getProximity(player_x, player_y, xpos,ypos);
            delete ct;
        }
    }

    void debugStats(Vec3b neigh, Vec3b pix, int k, int xpos, int ypos, float diam){
        fprintf(stderr,"P%02d@[%d,%d]\trad:%3f", k, xpos, ypos, diam);

        fprintf(stderr," {RGB=[%3d,%3d,%3d]",                             //Print RGB for center
                int(pix.val[2]),
                int(pix.val[1]),
                int(pix.val[0]));

        fprintf(stderr," Av(%dx%d)=[%3d,%3d,%3d]}",                       //Print RGB for average
                (neigh_size*2)+1, (2*neigh_size)+1,
                int(neigh.val[2]),
                int(neigh.val[1]),
                int(neigh.val[0]));
    }

    Vec3b neighPix(int &xpos, int &ypos, Mat &fgmask){
        Vec3b neigh;
        int num_neigh=0;
        for (int y=ypos-neigh_size; y <= ypos+neigh_size; y++)
        {
            for (int x=xpos-1; x <= xpos +1; x++) {
                Vec3b &pref = fgmask.at<Vec3b>(y,x);
                neigh += pref;
                num_neigh ++;
            }
        }
        neigh *= (float)(1/(float)(num_neigh));
        return neigh;
    }

    int getProximity(int &player_x, int &player_y, int &xpos, int &ypos){
        //1. How far away?
        int x_dist = player_x - xpos,  y_dist = player_y - ypos;
        return isqrt( int((x_dist*x_dist)+(y_dist*y_dist)) );
    }
};
