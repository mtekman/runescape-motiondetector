#ifndef DESKTOP_H
#define DESKTOP_H


#include "timeops.h"
#include "typedefs.h"
#include "blobops.h"
#include "dropzone.h"


#include <X11/Xlib.h>
#include <X11/Xutil.h>


struct DesktopOps {

    static Display *disp;
    static Window root;

    static Point window_coords;
    static Point window_dims;

    static Point inventory_dims;

    static void populateMat(Mat &img,
                            int &width = window_dims.x,
                            int &height= window_dims.y,
                            int &x=window_coords.x,
                            int &y=window_coords.y
                            )
    {
        XImage *xImageSample = XGetImage(disp, root, x, y, width, height, AllPlanes, ZPixmap);

        if (!(xImageSample != NULL && disp != NULL)) exit(-1);

        assert(xImageSample->format == ZPixmap);
        assert(xImageSample->depth == 24);

        IplImage *iplImage = cvCreateImageHeader(
                    cvSize(xImageSample->width, xImageSample->height),
                    IPL_DEPTH_8U,
                    xImageSample->bits_per_pixel/8);

        if(xImageSample->data != NULL){
            cvSetData(iplImage, xImageSample->data, xImageSample->bytes_per_line);
        }

        Mat src_img(iplImage);
        cvtColor(src_img,img,CV_BGRA2BGR); //Remove alpha in Ximage  4 channel --> 3


        cvReleaseImageHeader(&iplImage);
        XDestroyImage(xImageSample);
    }



    static Mat getInventory(){
        Mat invent;
        populateMat(invent, inventory_dims.x, inventory_dims.y);

        return invent;
    }


    static Point findOre(bool choose_random){

        if (choose_random){
            //Statically defined by inventory in top left corner
            // 3x3 window should be, each approx 40 pixels WxH

            int start_x = 26, start_y = 45;

            int col_index = rand()%3, row_index = rand()%3;
            int col_modif = col_index * 40;
            int row_modif = row_index * 40;

            return Point( start_x + col_modif,
                          start_y + row_modif);

        }

        // Else find the ore automagically
        Mat inventory = getInventory();
        return DropZone(inventory,ORE_TYPE).match;
    }


    static void dropOre(bool choose_random=false)
    {
        // Right click on Ore
        Point ore_point = findOre(choose_random);
        clickhere(ore_point.x, ore_point.y, 3);
        TimeOps::randsleep(1,2);

        //Drop it if random, else find the 'Drop' part
        if (choose_random){
            clickhere(ore_point.x, ore_point.y + 42, 1);

            TimeOps::randsleep(1,2);

            clickhere(ore_point.x, ore_point.y, 1); // left click off
        }
        else{
            Mat zone = getInventory();
            Point drop = DropZone(zone,DROP_TYPE).match;
            clickhere(drop.x, drop.y, 1);
        }
        TimeOps::randsleep(0,1);
    }


    static bool pickSignUp(){
        Mat top_center;

        int width = window_dims.x/4, height = window_dims.y/6;
        int startx = window_coords.x + (window_dims.x/2) - (width/2);
        int starty = window_coords.y + 0;

        populateMat(top_center, width, height, startx, starty);
        return DropZone(top_center,PICK_TYPE).match!=DropZone::pick_offset_xy;
    }


    static void clickOnOne(
            keyvect orelocs,
            bool click_nearest,
            Point2f player_coords)
    {
        KeyPoint at;

        if (click_nearest){
            float min_dist = 999999999999999;

            for (keyvect::iterator kit = orelocs.begin();
                 kit!=orelocs.end(); ++kit)
            {
                KeyPoint curr = *kit;

                float dist = BlobOps::dist_between(player_coords, curr.pt);
                cerr << "-- Dist=" << dist << "  between [" << player_coords.x << "," << player_coords.y
                     << "] and [" << curr.pt.x << "," << curr.pt.y << "]" << endl;
                if (dist < min_dist) {
                    min_dist=dist;
                    at = curr;
                }
            }
        }
        else at = orelocs.at(rand()%orelocs.size());
//        else at = orelocs[orelocs.size()-1];

        cerr << "-- choosing: [" << at.pt.x << "," << at.pt.y << "]" << endl;

        clickhere(at.pt.x, at.pt.y, 1);//, top_coords.x, top_coords.y);
    }

    static void charbychar(char * start, int len){

        for (int i=0; i < len; ++i){
            char t = start[i]; char buffer[20];

            sprintf(buffer,"xdotool key %c\n",t);
            TimeOps::randsleep(50,100);

            system(buffer);
        }
    }

    /** Type a random sequence of 1 2 3 **/
    static void typeHits(){
        char possibs[7]={'1','1','1','1','2','2','3'};

        int len=rand()%11; //0->11
        int i=len;

        char buff[len];
        while (i>0){
            char t = possibs[rand()%7];
            buff[len-i]=t;
            i--;
        }
        charbychar(buff,len);
    //    printf("\n",stderr);
    }


    static void clickhere(int x, int y, int mouse=1,
                          int topl_x = window_coords.x,
                          int topl_y = window_coords.y){
        char buffer[250];
        sprintf(buffer,"xdotool mousemove `expr %d + %d` `expr %d + %d` click %d\n",
                topl_x, x,
                topl_y, y,
                mouse);
//        printf(buffer,stderr);
        system(buffer);
    }
};


Display *DesktopOps::disp = XOpenDisplay(NULL);
Window DesktopOps::root = DefaultRootWindow(DesktopOps::disp);


//init to Point(0,0)
Point DesktopOps::window_coords;
Point DesktopOps::window_dims;

Point DesktopOps::inventory_dims;

#endif
