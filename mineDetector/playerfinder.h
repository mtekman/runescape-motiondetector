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

        Mat early_player = early(roi);
        Mat later_player = later(roi);

        Mat diff_player = later_player - early_player;

        Mat hsver[3];
        split(diff_player, hsver);
        Mat bw_diff = hsver[2]; // Value component == brightness == greyscale

        int erosion_size= 2;
        Mat element = getStructuringElement(MORPH_ELLIPSE,
                                            Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                            Point( erosion_size, erosion_size ) );

        erode(bw_diff, bw_diff, element);

        // Find the blue regiion..
        Mat get_blue;
        inRange(early_player, Scalar(90,50,50), Scalar(150,255,255), get_blue);

        //.. and remove it from analysis
//        cout << "Non-zero = " << countNonZero(bw_diff) << endl;

        bw_diff = bw_diff - get_blue;
        showIMG(bw_diff, "bw");

        if (debug){
            cout << "Non-zero = " << countNonZero(bw_diff) << endl;

            Mat debugger;
            Mat bw_diff_hsv;
            gray2HSV(bw_diff, bw_diff_hsv);

            vconcat(early_player, later_player, debugger);
            vconcat(debugger, diff_player, debugger);
            vconcat(debugger, bw_diff_hsv, debugger);


            showHSV(debugger, "debugger");
        }

        //Threshold of movement determined by average testing via movement_debugger.sh
        move_pix = countNonZero(bw_diff);
        is_idle = move_pix < 70;

        if (debug)
            cerr << "Player idle?" << is_idle << ", movePix:" << move_pix << endl;

    }
};

#endif
