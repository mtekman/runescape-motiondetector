#ifndef CVFUNCS_H
#define CVFUNCS_H

#include "typedefs.h"


#define DEBUG_KP(kpma)\
    for(keyvect::iterator kit= kpma.begin(); kit!= kpma.end(); ++kit){\
        KeyPoint kp = *kit;\
        cerr << " ([" << kp.pt.x << "," << kp.pt.y << "] " << kp.size << ")" << flush;\
    }

#define DEBUG_KM(kpma)\
    for(keymap::iterator kit= kpma.begin(); kit!= kpma.end(); ++kit){\
        float dist = kit->first;\
        KeyPoint kp = kit->second;\
        cerr << " (" << dist << "[" << kp.pt.x << "," << kp.pt.y << "] " << kp.size << ")" << flush;\
    }



struct CVFuncs{

    static Mat oneChannelto3(Mat &search_mask){
        Mat search_mask_img;
        Mat search_three_channel [] = {search_mask, search_mask, search_mask};
        merge(search_three_channel, 3, search_mask_img);
        return search_mask_img;
    }

    static Mat makeSearchMap2Mask(int width, int height, keymap &km){
        // Make searc mask image (Three channels)
        Mat search_mask = Mat::zeros(height, width, CV_8U);
        addBlobMap2Image(km,search_mask);
        return oneChannelto3(search_mask);
    }

    static Mat makeSearchVect2Mask(int width, int height, keyvect &kv){
        // Make searc mask image (Three channels)
        Mat search_mask = Mat::zeros(height, width, CV_8U);
        addBlobVect2Image(kv,search_mask);
        return oneChannelto3(search_mask);
    }


    static void addSearchMask2Img(Mat &debug_img, keymap &km, float intens=1){
        Mat search_mask = makeSearchMap2Mask(debug_img.cols, debug_img.rows, km);
        search_mask *= intens;
        debug_img += search_mask;
    }

    static void addSearchVect2Img(Mat &debug_img, keyvect &km, float intens=1){
        Mat search_mask = makeSearchVect2Mask(debug_img.cols, debug_img.rows, km);
        search_mask *= intens;
        debug_img += search_mask;
    }


    static void showKeyMapIMG(Mat &img, keymap &km){
        cerr << "KP::" << flush; DEBUG_KM(km); cerr << '\n' << endl;
        float scalef = 1.2;

        Mat debug_img(img);
        addSearchMask2Img(debug_img, km);

        resize(debug_img, debug_img, Size(img.cols/scalef, img.rows/scalef));

        showIMG(debug_img, 1050, 150);
    }

    static void showKeyVectIMG(Mat &img, keyvect &km){
        cerr << "KP::" << flush; DEBUG_KP(km); cerr << '\n' << endl;
        float scalef = 1.2;

        Mat debug_img(img);
        addSearchVect2Img(debug_img, km);

        resize(debug_img, debug_img, Size(img.cols/scalef, img.rows/scalef));

        showIMG(debug_img, 1050, 150);
    }


    static void addText2Image(float val, int x, int y, Mat &img){
        //Add text
        char s[5]; sprintf(s,"P %.4f", val);

        putText(img, s , cvPoint(x, y),
                FONT_HERSHEY_COMPLEX_SMALL, 0.5, cvScalar(200,200,250), 1, CV_AA);
    }


    static void addBlobVect2Image(keyvect &kv, Mat &image, ushort color=0){
        int count=0;
        for (keyvect::const_iterator itt = kv.begin(); itt != kv.end(); ++itt){
            KeyPoint ka = *itt;
            addBlob2Image(ka, image, color);
            addText2Image(ka.size, ka.pt.x, ka.pt.y, image);

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
