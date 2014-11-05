#ifndef OREOPS_H
#define OREOPS_H

//#define DEBUG


#define STATIC_DIR "static_imgs/"

#define ORE_TYPE "ore"
#define DROP_TYPE "drop"
#define PICK_TYPE "pick"

#define ORE_PLATE_NUM 4
#define DROP_PLATE_NUM 2

#include "typedefs.h"
#include "cvfuncs.h"

#define INVALID Point(10,10)


class DropZone {

    static Point drop_offset_xy;

    static Mat ore_template[ORE_PLATE_NUM];
    static Point ore_offset_xy;

    static Mat pick_template;
    static Mat drop_template[DROP_PLATE_NUM];

    Mat scan_area;
    int attempts;
public:
    static Point pick_offset_xy;

    Point match;

    DropZone(Mat &hsv_img, const char * type, int att=1)
    {
        attempts = att;
        cvtColor(hsv_img, scan_area, CV_HSV2BGR);

        int i=0;

        if (type==ORE_TYPE)
        {
            while(i<ORE_PLATE_NUM
                  && (match = find(ore_template[i],
                                   ore_offset_xy))==INVALID) i++;
        }
        else if (type==DROP_TYPE){
            //There are two types of DROP templates due to font kerning
            while(i<DROP_PLATE_NUM
                  && (match = find(drop_template[i],
                                   drop_offset_xy))==INVALID) i++;
        }
        else if (type==PICK_TYPE){
            match = find(pick_template, pick_offset_xy);
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
//        cerr << "img (rows,cols) " << scan_area.rows << "," << scan_area.cols << endl;
//        cerr << "tpl (rows,cols) " << temple.rows << "," << temple.cols << endl;

        matchTemplate( scan_area, temple, result, CV_TM_SQDIFF_NORMED );
        normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

        // Localizing the best match with minMaxLoc
        double minVal; double maxVal; Point minLoc; Point maxLoc;
        minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

//        cerr << minVal << " -- " << maxVal << endl;
        if (minVal==0) return INVALID;
        return minLoc + offset;
    }


    inline Point find(Mat &temple, Point &offset){
        int ntabc = attempts;
        while(--ntabc > 0){
//            cerr << ntabc << "  " << endl;
            Point res = track(temple, offset);
            if (res!=INVALID) return res;
        }
        return INVALID;
    }
};



// Need to fix this, am using the same tempate twice
Mat DropZone::drop_template[DROP_PLATE_NUM] =\
 {imread(STATIC_DIR "drop_template.jpg"),
  imread(STATIC_DIR "drop_template2.jpg")};

Point DropZone::drop_offset_xy = Point(
            DropZone::drop_template[0].cols/2,
            DropZone::drop_template[0].rows/2);



Mat DropZone::ore_template[ORE_PLATE_NUM] =\
{imread(STATIC_DIR "ore_template.jpg"),
 imread(STATIC_DIR "ore_template2.jpg"),
 imread(STATIC_DIR "ore_template_sub1.jpg"),
 imread(STATIC_DIR "ore_template_sub2.jpg")};

Point DropZone::ore_offset_xy = Point(
            DropZone::ore_template[0].cols/2,
            DropZone::ore_template[0].rows/2);


Mat DropZone::pick_template =\
        imread(STATIC_DIR "pick_template.jpg");


Point DropZone::pick_offset_xy = Point(
            DropZone::pick_template.cols/2,
            DropZone::pick_template.rows/2);


#endif
