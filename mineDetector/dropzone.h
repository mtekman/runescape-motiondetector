#ifndef OREOPS_H
#define OREOPS_H

#define DEBUG

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

    Mat scan_area;
public:
    static Point pick_offset_xy;

    Point match;

    DropZone(Mat &hsv_img, const char * type)
    {
        cvtColor(hsv_img, scan_area, CV_HSV2BGR);

        if (type==ORE_TYPE){
            match = track(ore_template, ore_offset_xy);
        }
        else if (type==DROP_TYPE){
            //There are two types of DROP templates due to font kerning

            match = track(drop_template[0], drop_offset_xy);
            if (match==INVALID)
                match = track(drop_template[1], drop_offset_xy);

        }
        else if (type==PICK_TYPE){
            match = track(pick_template, pick_offset_xy);
        }

        else {
            cerr << "Problem..." << endl;
            exit(-1);
        }

#ifdef DEBUG
        KeyPoint kp;
        kp.pt = match;
        kp.size = 5;

        cerr << "Match at=" << match.x << "," << match.y << endl;
        showHSV(scan_area, "scan");
        CVFuncs::addBlob2Image(kp, scan_area, 0);
        showHSV(scan_area, "blobs");
#endif
    }

private:
    Point track(Mat &temple, Point &offset)
    {
        Mat result;
        cerr << "img (rows,cols) " << scan_area.rows << "," << scan_area.cols << endl;
        cerr << "tpl (rows,cols) " << temple.rows << "," << temple.cols << endl;

        matchTemplate( scan_area, temple, result, CV_TM_SQDIFF_NORMED );
        normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

        // Localizing the best match with minMaxLoc
        double minVal; double maxVal; Point minLoc; Point maxLoc;
        minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

        cerr << minVal << " -- " << maxVal << endl;
        if (minVal==0) return INVALID;
        return minLoc + offset;
    }


};

// Need to fix this, am using the same tempate twice
Mat DropZone::drop_template[2] =\
 {imread("drop_template.jpg"), imread("drop_template2.jpg")};

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
