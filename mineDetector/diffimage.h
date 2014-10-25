#ifndef DIFFIMAGE_H
#define DIFFIMAGE_H

#include "typedefs.h"

class DiffImage
{
public:
    Mat fgmask;
    bool empty;

    DiffImage(Mat &early, Mat &later){
        empty=false;

        difference(early,later);

        if (notEmpty()){}
        else empty=true;
    }

private:
    bool notEmpty(){
        Scalar sums = sum(fgmask);
        if ((sums.val[0] > 10 ) || (sums.val[1] > 10) || (sums.val[2]>10) ) return true;
        return false;
    }


    void difference(Mat &early,Mat &later)
    {
        Mat fgmask1 = later - early;      //Simple subtraction
        Mat fgmask2 = early - later;

        fgmask = fgmask1 + fgmask2;

        showHSV(fgmask, "1+2");

        fgmask /= 50;
        fgmask *= 50;

        showHSV(fgmask, "posterize");

        int dilation_size = 4;

        dilate(fgmask, fgmask, getStructuringElement(MORPH_ELLIPSE,
                                                     Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                                     Point( dilation_size, dilation_size ) ));

        showHSV(fgmask, "dilated");

        // Filter out non-whites
        //For HSV we need only to filter for S and V:
        //  -- low S and high V == white
        //  -- any S and low V == black
        Mat splitter[3];
        split(fgmask,splitter);

        //Low saturation of any hue or brightness will be grey
//        Mat white_grey_black = splitter[0] & (splitter[1]<50) & splitter[2];

//        Mat white = splitter[0] & (splitter[1]<50) & (splitter[2]>200);

        // Low luminosity of any hue or saturation will still be black
//        Mat black = splitter[0] & splitter[1] & (splitter[2]<50);

        fgmask = splitter[2];
//        showIMG(fgmask, "black points only");
    }
};

#endif


/*
cerr << "fighta" << endl;
showIMG(diff_player);


//Extract blue channel as a binary image {0,255}
Mat blue;
inRange(later, Scalar(90,50,50), Scalar(150,255,255), blue);

Mat blue_filler = Mat::ones(int(blue.rows), int(blue.cols), CV_8U);

Mat all;
merge({blue,blue_filler, blue_filler}, all);

cerr <<  "sore?" << endl;
showHSV(later);
showIMG(all);
showHSV(later);


Mat new_later = later - all; // minus 255 from value
cerr << "here?" << endl;

showHSV(new_later);
exit(0);
*/
