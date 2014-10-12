#ifndef PLAYERF_H
#define PLAYERF_H

#include "typedefs.h"

struct PlayerFinder{
    bool is_idle;
    int move_pix;

    PlayerFinder(Point &player_coords, Mat &early, Mat &later, bool debug)
    {
        //Define ROI (region of interest)
        int height = early.rows/10, width= early.cols/10;

        Rect roi(
                    player_coords.x - width/2,
                    player_coords.y - height/2,
                    width,
                    height);

        Mat early_player = Mat(early,roi);
        Mat later_player = Mat(later, roi);

        Mat diff_player = later_player - early_player;

        //Num white pixels, serial method
        Mat bw_diff;
        cvtColor(diff_player, bw_diff, CV_RGB2GRAY);

        int erosion_size= 2;
        Mat element = getStructuringElement(MORPH_CROSS,
                                            Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                            Point( erosion_size, erosion_size ) );

        erode(bw_diff, bw_diff, element);

//        if(debug){
//            cout << "Non-zero = " << countNonZero(bw_diff) << endl;

//            Mat debugger;
//            vconcat(early_player, later_player, debugger);
//            vconcat(debugger, diff_player, debugger);

//            showIMG(debugger, 1300, 10);
//        }

        //Threshold of movement determined to be at 60 by testing
        move_pix = countNonZero(bw_diff);
        is_idle = move_pix<40;
    }
};

#endif
