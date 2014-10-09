#ifndef CVFUNCS_H
#define CVFUNCS_H

#include <X11/Xlib.h>

#include "typedefs.h"

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


    static void addText2Image(int val, int x, int y, Mat &img){
        //Add text
        char s[5]; sprintf(s,"P %02d", val);

        putText(img, s , cvPoint(x, y),
                FONT_HERSHEY_COMPLEX_SMALL, 0.5, cvScalar(200,200,250), 1, CV_AA);
    }


    static void addBlobVect2Image(keyvect &kv, Mat &image, ushort color=0){
        for (keyvect::const_iterator itt = kv.begin(); itt != kv.end(); ++itt){
            KeyPoint ka = *itt;
            addBlob2Image(ka, image, color);
        }
    }



    static void addBlobMap2Image(keymap &kv, Mat &image, ushort color=0){
        for (keymap::const_iterator itt = kv.begin(); itt != kv.end(); ++itt){
            float dist = itt->first;
            KeyPoint ka = itt->second;
            addBlob2Image(ka, image, color);
            addText2Image((int)dist, ka.pt.x, ka.pt.y, image);
        }
    }

    static void addBlob2Image(KeyPoint &kp, Mat &image, ushort color=0){
        int rad = int(kp.size);                                       //Update thumbnail image with spots left
        int xpos = int(kp.pt.x), ypos = int(kp.pt.y);

        int scalef = 1; //... uh...
        int ypos_sf = ypos/scalef, xpos_sf = xpos/scalef;

        // color:   0 --> white
        //          1 --> red
        //          2 --> green
        //          3 --> yellow


        for (int i=-rad; i < rad; i++){
            for (int j=-rad; j < rad; j++){
                Vec3b &pixval2 = image.at<Vec3b>((ypos_sf)+j,(xpos_sf)+i);  //Default Green = good
                pixval2.val[2] = pixval2.val[1] = pixval2.val[0] = uchar(0); //Black

                switch(color){
                    case 0: // white
                        pixval2.val[2] = pixval2.val[1] = pixval2.val[0] = uchar(200); //white
                        break;
                    case 1: // red
                        pixval2.val[0] = uchar(200);
                        break;
                    case 2: // green
                        pixval2.val[1] = uchar(200);
                        break;
                    case 3: // yellow
                        pixval2.val[0] = pixval2.val[1] = uchar(200);
                        break;
                }
            }
        }
    }


};


#endif
