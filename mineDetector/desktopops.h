#ifndef DESKTOP_H
#define DESKTOP_H

#include "timeops.h"
#include "typedefs.h"

struct DesktopOps{

    static void clickOnOne(keyvect orelocs, bool click_nearest){}

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


    static void clickhere(int topl_x, int topl_y, int x, int y){
        char buffer[250];
        sprintf(buffer,"xdotool mousemove `expr %d + %d` `expr %d + %d` click 1\n",
                topl_x, x,
                topl_y, y);
    //    printf(buffer,stderr);
        system(buffer);
    }
};

#endif
