#ifndef CVFUNCS_H
#define CVFUNCS_H

#include <X11/Xlib.h>
#include <opencv/cv.h>

using namespace cv;


struct CVFuncs{

    static int populateMat(Mat &img, Display *disp, Window &root,
                           int &x, int &y, int &width, int& height)
    {
        XImage* xImageSample = XGetImage(disp, root, x, y, width, height, AllPlanes, ZPixmap);

        if (!(xImageSample != NULL && disp != NULL)) return EXIT_FAILURE;

        assert(xImageSample->format == ZPixmap);
        assert(xImageSample->depth == 24);

        IplImage *iplImage = cvCreateImageHeader(
                    cvSize(xImageSample->width, xImageSample->height),
                    IPL_DEPTH_8U,
                    xImageSample->bits_per_pixel/8);

        iplImage->widthStep = xImageSample->bytes_per_line;
        if(xImageSample->data != NULL)
            iplImage->imageData = xImageSample->data;

        img = Mat(iplImage);
        cvtColor(img,img,CV_BGRA2BGR); //Remove alpha in Ximage  4 channel --> 3

        return 0;
    }

};


#endif
