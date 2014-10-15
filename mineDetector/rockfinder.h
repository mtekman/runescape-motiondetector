#ifndef ROCKFIND_H
#define ROCKFIND_H

#include "typedefs.h"

typedef vector<Vec3f> circlevect;

class RockFinder{
public:
    circlevect circles;

    RockFinder(Mat &fgmask){
        /// Convert it to gray
        Mat grey_mask;
        cvtColor( fgmask, grey_mask, CV_BGR2GRAY );

        /// Reduce the noise so we avoid false circle detection
        GaussianBlur( grey_mask, grey_mask, Size(9, 9), 2, 2 );

        /// Apply the Hough Transform to find the circles
        int circle_radius = grey_mask.rows/10;


        HoughCircles( grey_mask, circles, CV_HOUGH_GRADIENT, 100, 0.0001);

        /// Draw the circles detected
        for( size_t i = 0; i < circles.size(); i++ )
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);

            cerr << "P" << i << ":" << center.x << "," << center.y << "=" << radius << endl;

            // circle center
            circle( fgmask, center, 3, Scalar(0,255,0), -1, 8, 0 );
            // circle outline
            circle( fgmask, center, radius, Scalar(0,0,255), 3, 8, 0 );
        }

        /// Show your results
        namedWindow( "Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE );
        imshow( "Hough Circle Transform Demo", fgmask );
        moveWindow("Hough Circle Transform Demo", 900, 100);

        waitKey(0);
    }
};

#endif
