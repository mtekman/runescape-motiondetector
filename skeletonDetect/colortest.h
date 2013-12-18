#include <stdio.h>
#include <opencv2/core/core.hpp>


using namespace cv;

class ColorTest
{
private:
    float red_to_green, red_to_blue;
    bool debug;

    bool isRed(Vec3b &neigh){
        red_to_green = (float)(neigh.val[2])/(float)(neigh.val[1]);
        red_to_blue  = (float)(neigh.val[2])/(float)(neigh.val[0]);

        if ((red_to_green > 2) && (red_to_blue > 2) && (neigh.val[2]>60))    {  //If R twice dominates other channels and is intense,
            if(debug) fprintf(stderr," Red!");
            red=true;
            return true;
        }
        return false;
    }


    bool isYellow(Vec3b &neigh){
        //float &yellow_ratio = red_to_green;
        if ((red_to_green > 0.6) && (neigh.val[1] > neigh.val[0]) && (neigh.val[2]>60))    {     //If RG channels related, and G value greater than B, and RG channel>60 (intense)
            if(debug) fprintf(stderr," Yellow!");
            yellow=true;
            return true;
        }
        return false;
    }

    bool passTest(Vec3b neigh){
        if(isRed(neigh)) return false;
        if(isYellow(neigh)) return false;
        return true;
    }


public:
    bool pass, yellow, red;

    ColorTest(Vec3b neigh, bool debug){
        this->debug = debug;
        red_to_blue = red_to_green = -1;
        pass = yellow = red = false;
        pass = passTest(neigh);
    }
};
