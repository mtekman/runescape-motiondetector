#ifndef OREOPS_H
#define OREOPS_H

//#define DEBUG

#define ORE_TYPE "ore"
#define DROP_TYPE "drop"
#define PICK_TYPE "pick"

#include "typedefs.h"
#include "cvfuncs.h"

#define INVALID Point(10,10)


class DropZone {

    static Point drop_offset_xy;

    static Mat ore_template;
    static Point ore_offset_xy;

    static Mat pick_template;
    static Mat drop_template[2];

public:
    static Point pick_offset_xy;

    Point match;

    DropZone(Mat &scan_area, const char * type)
    {
        if (type==ORE_TYPE){
            match = track(scan_area, ore_template, ore_offset_xy);
        }
        else if (type==DROP_TYPE){
            //There are two types of DROP templates due to font kerning

            match = track(scan_area, drop_template[0], drop_offset_xy);
            if (match==INVALID)
                match = track(scan_area, drop_template[1], drop_offset_xy);

        }
        else if (type==PICK_TYPE){
            match = track(scan_area, pick_template, pick_offset_xy);
        }

        else {
            cerr << "Problem..." << endl;
            exit(-1);
        }

#ifdef DEBUG
        if (type==DROP_TYPE){
            KeyPoint kp;
            kp.pt = match;
            kp.size = 5;

            showIMG(scan_area,900,0);
            CVFuncs::addBlob2Image(kp, scan_area, 0);
            showIMG(scan_area,900,0);
        }
#endif
    }

private:
    Point track(Mat &img, Mat &temple, Point &offset)
    {
        Mat result;
//        cerr << "img rows = " << img.rows << ", cols=" << img.cols << endl;
//        cerr << "tpl rows = " << temple.rows << ", cols=" << temple.cols << endl;
        matchTemplate( img, temple, result, CV_TM_SQDIFF_NORMED );
        normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

        // Localizing the best match with minMaxLoc
        double minVal; double maxVal; Point minLoc; Point maxLoc;
        minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

        if (minVal==0) return INVALID;
//        cerr << minVal << " -- " << minVal << endl;
        return minLoc + offset;
    }


};


Mat DropZone::drop_template[2] =\
 {imread("drop_template3.jpg"), imread("drop_template.jpg")};

Point DropZone::drop_offset_xy = Point(
            DropZone::drop_template[0].cols/2,
            DropZone::drop_template[0].rows/2);



Mat DropZone::ore_template = imread("ore_template.jpg");

Point DropZone::ore_offset_xy = Point(
            DropZone::ore_template.cols/2,
            DropZone::ore_template.rows/2);


Mat DropZone::pick_template = imread("pick_template.jpg");

Point DropZone::pick_offset_xy = Point(
            DropZone::pick_template.cols/2,
            DropZone::pick_template.rows/2);


#endif
