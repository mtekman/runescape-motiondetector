#define DISPLAY  //imshow prompts

#define neigh_size 2

#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void usage(){
    fprintf(stderr, "\nusage: skeletonDetect <first.jpg> <second.jpg> [--debug]");
    fprintf(stderr, "\n\tPrint X,Y coordinate of largest detected motion between two images\n\n");
    exit(-1);
}

int isqrt(int num) {
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



int main(int argc, char ** argv)
{

    Mat early, later;
//    int mask = 10;
    bool debug=false;

    if (argc<3 || argc>4) usage();

    if (argc >= 3){
        early = imread(argv[1]);
        later=  imread(argv[2]);
  //      mask = atoi(argv[3]);
        if (argc==4) debug = true;
    }


    Mat fgmask;

    // POSTERIZE
    early /=100;
    early *=100;

    later /=100;
    later *=100;


    //Simple subtraction
    fgmask = later - early;


#ifdef DISPLAY
    Mat debug_img;                    //Debug thumbnail, work is only projected here, not read from ever
    float scalef=1.8;                     //Scale factor of thumbnail

    if (debug){
        resize(fgmask, debug_img, Size(fgmask.cols/scalef, fgmask.rows/scalef));
        imshow("Frame",debug_img);
        waitKey(0);
    }
#endif


    /////////// REMOVE DISTRACTING ELEMENTS ////////////////////////////////////

    // REMOVE PLAYER  =  Black out center of screen
    int row_rad = fgmask.rows/2,     col_rad = fgmask.cols/2;
    int block_size_row = row_rad/4,  block_size_col = col_rad/6;
    int offset_X = 0,               offset_Y = -50;

    for (int y= offset_Y + row_rad - block_size_row; y< row_rad + block_size_row; y++)
    {
        for (int x= offset_X + col_rad - block_size_col; x < col_rad + block_size_col; x++) {
            Vec3b &pixels = fgmask.at<Vec3b>(y,x);
            pixels.val[0] = pixels.val[1] = pixels.val[2] = uchar(0);
        }
    }

    // REMOVE BOTTOM PANEL // remove 1/8th
    for (int y= fgmask.rows-(fgmask.rows/8); y< fgmask.rows; y++)
    {
        for (int x= 0; x < fgmask.cols; x++)
        {
            Vec3b &pixels = fgmask.at<Vec3b>(y,x);
            pixels.val[0] = pixels.val[1] = pixels.val[2] = uchar(0);
        }
    }

/*  //DONT erode, it ruins detection
    Mat element = getStructuringElement( MORPH_CROSS, Size(2,2));
    erode(fgmask,fgmask, element);
*/

#ifdef DISPLAY
    if (debug) resize(later, debug_img, debug_img.size());
#endif


    /////   BEGIN MOTION DETECTION

    SimpleBlobDetector::Params params;                                      // make blob
    params.filterByInertia = false;
    params.filterByConvexity = false;
    params.filterByColor = false;
    params.minDistBetweenBlobs = 1.0f;
/*    uchar pixels[3] = {100,100,100}; //RGB
    params.blobColor = *pixels;*/
    params.filterByCircularity = false;
    params.filterByArea = true;
    params.minArea = 5.0f;
    params.maxArea = 1000.0f;

    cv::Ptr<cv::FeatureDetector> blob_detector = new cv::SimpleBlobDetector(params);
    blob_detector->create("SimpleBlob");

    vector<cv::KeyPoint> keypoints;
    blob_detector->detect(fgmask, keypoints);

    map<float,KeyPoint> sorted_sizes;  //Sorts on key automatically (asc)

    // Sort Blobs from biggest to smallest
    for (int k=0; k<keypoints.size(); k++)
    {
        KeyPoint &kp = keypoints[k];
        sorted_sizes[kp.size]=kp;
    }



    //TEMPS
    float max_diam=1; KeyPoint max;                         //largest_blob
    int k_index_max= -1;                                    //which point

    int sq_distance_from_player = 1 << 30;                   //tracks how far away blob is from player/centre
    int player_y = row_rad*3/4, player_x =  col_rad;            //player position


    int k=-1;
    for(map<float,KeyPoint>::const_reverse_iterator kkk = sorted_sizes.rbegin(); kkk != sorted_sizes.rend(); ++kkk)
    {
        KeyPoint kp = (*kkk).second;

        //Kill after first 10 points
        if(k++==11) break;

        int xpos = int(kp.pt.x);
        int ypos = int(kp.pt.y);

        float diam = kp.size;
        Vec3b &pix = fgmask.at<Vec3b>(ypos,xpos);

        //Average Neighbouring pixels (3x3) --> Get what average color of spot focus is
        Vec3i neigh;
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


        //DEBUG Stats
        if (debug){
            fprintf(stderr,"P%02d@[%d,%d]\trad:%3f", k, int(xpos), int(ypos), diam);

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


        /////////////////////////////////////////////////// LET THE TESTS BEGIN! ////////////////////////////////////////////////////////
        bool new_max = false;

        ////////////////////////////// FLAME TESTS ///////////////////////////////////////////////////////////////
        //Bright RED TEST
        bool red=false;

        float red_to_green = (float)(neigh.val[2])/(float)(neigh.val[1]);
        float red_to_blue  = (float)(neigh.val[2])/(float)(neigh.val[0]);

        if ((red_to_green > 2) && (red_to_blue > 2) && (neigh.val[2]>60))    {  //If R twice dominates other channels and is intense,
            if(debug) fprintf(stderr," Red!");
            red=true;
        }

        //Bright YELLOW TEST =  RED:GREEN
        bool yellow=false;
        if (!red){
            float &yellow_ratio = red_to_green;

            if ((yellow_ratio > 0.6) && (neigh.val[1] > neigh.val[0]) && (neigh.val[2]>60))    {     //If RG channels related, and G value greater than B, and RG channel>60 (intense)
                if(debug) fprintf(stderr," Yellow!");
                yellow=true;
            }
        }


        ////////////////////////////// PROXIMITY TEST ////////////////////////////////////////////////////////////
/*      LOGIC:
        Each blob checked is smaller than the last.

        smaller blob closer?No  --> skip
                           Yes --> how much smaller? <= 0.5 ---> skip
                                                     >= 0.85 --> UPDATE
                                                     0.5 < x < 0.85  ----> how much closer? > 0.67 --> skip
                                                                                            <=0.67 ---> UPDATE
 */
        //1. How far away?
        int x_dist = player_x - xpos,  y_dist = player_y - ypos;
        int sq_distance = isqrt( int((x_dist*x_dist)+(y_dist*y_dist)) );

        if (!red && !yellow){

            if(debug) fprintf(stderr, " closer:%d", sq_distance);

            //2. Smaller blob closer?
            if(sq_distance > sq_distance_from_player){
                if(debug) fprintf(stderr, ":N");
                else continue;

            } else {
                if(debug) fprintf(stderr, ":Y");

                //3. How much smaller?
                float scale_size = diam/max_diam;
                if(debug) fprintf(stderr, " s_scale:%02f", scale_size);

                if(scale_size >= 0.85) {
                    if (debug) fprintf(stderr,":Y");
                    new_max=true;
                }
                else if (scale_size <= 0.5){
                    if (debug) fprintf(stderr,":N");
                    else continue;
                }
                else { //else if( (0.5 < scale_size) && (scale_size<0.85)){
                    if (debug) fprintf(stderr,":N");

                    //4. How much closer?
                    float dist_scale = (float)(sq_distance)/sq_distance_from_player;
                    if (debug) fprintf(stderr," d_scale:%02f", dist_scale);

                    if(dist_scale <= 0.67 ) {
                        if(debug)fprintf(stderr,":Y");
                        new_max=true;
                    }
                    else{
                        if(debug)fprintf(stderr,":N");
                        else continue;
                    }
                }
            }
        }

        if (new_max) {
            sq_distance_from_player = sq_distance;
            max_diam = diam;
            max = kp;
            k_index_max = k;
            if (debug) fprintf(stderr, "\n--->New Max @ P%02d",k);
        }




        if(debug){
#ifdef DISPLAY
            int rad = kp.size;                                       //Update thumbnail image with spots left
            int ypos_sf = ypos/scalef, xpos_sf = xpos/scalef;

            for (int i=-rad; i < rad; i++){
                for (int j=-rad; j < rad; j++){
                    Vec3b &pixval2 = debug_img.at<Vec3b>((ypos_sf)+j,(xpos_sf)+i);  //Default Green = good
                    pixval2.val[1] = uchar(255);
                    pixval2.val[2] = pixval2.val[0] = uchar(0);

                    if (yellow) pixval2.val[2] = uchar(255);   //Yellow (+red)
                    if(red) {
                        pixval2.val[2] = uchar(255);
                        pixval2.val[1] = uchar(0);
                    }

               }
            }

            //Add text
            char s[5]; sprintf(s,"P%02d", k);

            putText(debug_img, s , cvPoint(xpos_sf, ypos_sf),
                FONT_HERSHEY_COMPLEX_SMALL, 0.5, cvScalar(200,200,250), 1, CV_AA);

            //Add center point
            Vec3b &pixval2 = debug_img.at<Vec3b>(ypos_sf,xpos_sf);
            pixval2.val[0] = uchar(255);
            pixval2.val[1] = pixval2.val[2] = uchar(0);


            //Add line
            line( debug_img, Point(xpos_sf,ypos_sf), Point(player_x/scalef,player_y/scalef), Scalar(100,100,100),1); //No alpha channel cant transp

#endif
            fprintf(stderr,"\n");
        }
    }

#ifdef DISPLAY
    //Color max point DARK GREEN
    if(debug){
        int rad = max.size;
        for (int i=-rad; i < rad; i++){
            for (int j=-rad; j < rad; j++){
                Vec3b &pixval2 = debug_img.at<Vec3b>((max.pt.y/scalef)+j,(max.pt.x/scalef)+i);
                pixval2.val[1]  = uchar(50);
                pixval2.val[2] = pixval2.val[0] = uchar(0);
            }
        }

        //color centre of max RED
        Vec3b &centre = debug_img.at<Vec3b>(max.pt.y/scalef, max.pt.x/scalef);
        centre.val[0] = centre.val[1] = 0;
        centre.val[2] = uchar(255);

        imshow("Frame",debug_img);
        waitKey(0);
    }
#endif

    if (debug) fprintf(stderr, "\nLargest movement @ P%02d: ", k_index_max);
    fprintf(stdout, "%d %d\n", int(max.pt.x), int(max.pt.y));
}

