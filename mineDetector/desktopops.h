#ifndef DESKTOP_H
#define DESKTOP_H

#include "timeops.h"
#include "typedefs.h"
#include "blobops.h"

struct DesktopOps{

    static void dropOre(Point &window_coords){
        //Statically defined by inventory in top left corner
        // 3x3 window should be, each approx 40 pixels WxH

        int start_x = 26, start_y = 45;

        int col_index = rand()%3, row_index = rand()%3;
        int col_modif = col_index * 40;
        int row_modif = row_index * 40;

        clickhere(window_coords.x, window_coords.y,
                  start_x + col_modif,
                  start_y + row_modif, 3); // right click menu
        TimeOps::randsleep(0,1);
        clickhere(window_coords.x, window_coords.y,
                  start_x + col_modif,
                  start_y + row_modif + 42, 1); // left click drop
        TimeOps::randsleep(1,2);
        clickhere(window_coords.x, window_coords.y,
                  start_x + col_modif,
                  start_y + row_modif, 1); // left click off
        TimeOps::randsleep(0,1);
    }


    static void clickOnOne(keyvect orelocs, bool click_nearest, Point top_coords, Point2f player_coords)
    {
        KeyPoint at;

        if (click_nearest){
            float min_dist = 1 << 30;

            for (keyvect::iterator kit = orelocs.begin(); kit!=orelocs.end(); ++kit){
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

        clickhere(top_coords.x, top_coords.y, at.pt.x, at.pt.y);
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


    static void clickhere(int topl_x, int topl_y, int x, int y, int mouse=1){
        char buffer[250];
        sprintf(buffer,"xdotool mousemove `expr %d + %d` `expr %d + %d` click %d\n",
                topl_x, x,
                topl_y, y,
                mouse);
    //    printf(buffer,stderr);
        system(buffer);
    }
};

#endif
