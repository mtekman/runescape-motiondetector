#ifndef LOCATE_DROP
#define LOCATE_DROP

#include "cvfuncs.h"

#define DEBUG

class DropZone{
public:
    static Mat temple;
    Point match;

    DropZone(Mat &img_buffer){
        match = track(img_buffer);

#ifdef DEBUG
        KeyPoint kp;
        kp.pt = match;
        kp.size = 1;

        showIMG(img_buffer,900,0);
        CVFuncs::addBlob2Image(kp, img_buffer, 0);
        showIMG(img_buffer,900,0);

#endif
    }


    Point track(Mat &img)
    {
        Mat result;
        /// Do the Matching and Normalize
        ///

        cerr << "img rows = " << img.rows << ", cols=" << img.cols << endl;
        cerr << "tpl rows = " << temple.rows << ", cols=" << temple.cols << endl;

        matchTemplate( img, temple, result, CV_TM_SQDIFF_NORMED );
        normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

        /// Localizing the best match with minMaxLoc
        double minVal; double maxVal; Point minLoc; Point maxLoc;
        minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

        return minLoc;
    }
};

// static once
Mat DropZone::temple = imread("drop_template.jpg");


#endif
