//#define DISPLAY  //imshow prompts

#define neigh_size 2

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "blobmaster.h"
#include "blobtest.h"
#include "diffimage.h"

using namespace std;


class Detector
{
public:
    //TEMPS
    float max_diam; KeyPoint max;                         //largest_blob
    int k_index_max;                                    //which point

    int sq_distance_from_player;                   //tracks how far away blob is from player/centre
    int player_y, player_x;            //player position
    int row_rad, col_rad;               //Center of img

    Mat fgmask; //forw decl.

#ifdef DISPLAY
    Mat debug_img;            // Debug thumbnail, work is only projected here, not read from ever
    float scalef;               // Scale factor of thumbnail
#endif

    Detector(bool debug, Mat &early, Mat &later){
#ifdef DISPLAY
        scalef= 1.8;
#endif

        max_diam = 1;
        k_index_max = -1;
        sq_distance_from_player = 1<<30;
        player_y = early.rows*3/4;
        player_x =  early.cols;


        fgmask = DiffImage(early,later).fgmask;
        moDetect(fgmask, debug);
    }

    void moDetect(Mat &fgmask, bool debug)
    {

#ifdef DISPLAY
        if (debug){
            resize(fgmask, debug_img, Size(fgmask.cols/scalef, fgmask.rows/scalef));
            imshow("Frame",debug_img);
            waitKey(0);
        }
#endif
        map<float,KeyPoint> sorted_sizes = BlobMaster(fgmask).sorted_sizes;
        int k_counter=0;

        for(map<float,KeyPoint>::const_reverse_iterator\
            kkk = sorted_sizes.rbegin();
            kkk != sorted_sizes.rend();
            ++kkk)
        {
            k_counter ++;
            KeyPoint kp = (*kkk).second;
            TestEachBlob teb(kp,fgmask, player_x, player_y, k_counter, debug);

            int &proxy  = teb.proxim;

            if(proxy==-1) continue; //Not valid for consideration

#ifdef DISPLAY
            if (debug) addBlobToDebugIMG(kp, k_counter, teb.ct->yellow,teb.ct->red);
#endif
            float &diam = kp.size;

            if(isNewMax(proxy, diam, debug))
                updateMax(proxy,diam,kp,k_counter, debug);

        }


#ifdef DISPLAY
        if(debug) {
            addBlobToDebugIMG(max, k_index_max, false,false,true);
            imshow("Frame",debug_img);
            waitKey(0);
        }
#endif

        if (debug) fprintf(stderr, "\nLargest movement @ P%02d: ", k_index_max);
        fprintf(stdout, "%d %d\n", int(max.pt.x), int(max.pt.y));

    }


    void addBlobToDebugIMG(KeyPoint kp, int k,
                           bool yellow, bool red, bool max=false){
        int rad = int(kp.size);                                       //Update thumbnail image with spots left
        int xpos = int(kp.pt.x), ypos = int(kp.pt.y);

        int ypos_sf = ypos/scalef, xpos_sf = xpos/scalef;

        for (int i=-rad; i < rad; i++){
            for (int j=-rad; j < rad; j++){
                Vec3b &pixval2 = debug_img.at<Vec3b>((ypos_sf)+j,(xpos_sf)+i);  //Default Green = good
                pixval2.val[1] = uchar(255);
                pixval2.val[2] = pixval2.val[0] = uchar(0);

                if(!max){
                    if (yellow) pixval2.val[2] = uchar(255);   //Yellow (+red)
                    if (red) {
                        pixval2.val[2] = uchar(255);
                        pixval2.val[1] = uchar(0);
                    }
                }
            }
        }

        //Add text
        char s[5]; sprintf(s,"P%02d", k);

        putText(debug_img, s , cvPoint(xpos_sf, ypos_sf),
                FONT_HERSHEY_COMPLEX_SMALL, 0.5, cvScalar(200,200,250), 1, CV_AA);

        //Add center point
        Vec3b &pixval2 = debug_img.at<Vec3b>(ypos_sf,xpos_sf);
        if(max){
            pixval2.val[0] = pixval2.val[1] = 0;
            pixval2.val[2] = uchar(255);
        }
        else{
            pixval2.val[0] = uchar(255);
            pixval2.val[1] = pixval2.val[2] = uchar(0);
        }

        //Add line
        line( debug_img, Point(xpos_sf,ypos_sf), Point(player_x/scalef,player_y/scalef), Scalar(100,100,100),1); //No alpha channel cant transp

        fprintf(stderr,"\n");

    }

    bool isNewMax(int &sq_distance, float &diam, bool &debug){
        if(debug) fprintf(stderr, " closer:%d", sq_distance);

        //2. Smaller blob closer?
        if(sq_distance > sq_distance_from_player){
            if(debug) fprintf(stderr, ":N");
            return false;
        }

        if(debug) fprintf(stderr, ":Y");

        //3. How much smaller?
        float scale_size = diam/max_diam;
        if(debug) fprintf(stderr, " s_scale:%02f", scale_size);

        if(scale_size >= 0.85) {
            if (debug) fprintf(stderr,":Y");
            return true;
        }

        if (scale_size <= 0.5){
            if (debug) fprintf(stderr,":N");
            else return false;
        }

        if (debug) fprintf(stderr,":N");

        //4. How much closer?
        float dist_scale = (float)(sq_distance)/sq_distance_from_player;
        if (debug) fprintf(stderr," d_scale:%02f", dist_scale);

        if(dist_scale <= 0.67 ) {
            if(debug)fprintf(stderr,":Y");
            return true;
        }

        if(debug)fprintf(stderr,":N");
        return false;
    }

    void updateMax(float sq_distance, float diam, KeyPoint kp, int k, bool &debug){
        sq_distance_from_player = sq_distance;
        max_diam = diam;
        max = kp;
        k_index_max = k;
        if (debug) fprintf(stderr, "\n--->New Max @ P%02d",k);
    }

};
