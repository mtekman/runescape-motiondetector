#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


class DiffImage
{
public:
    Mat fgmask;
    bool empty;

    DiffImage(Mat &early, Mat &later){
        empty=false;

        difference(early,later);

        if (notEmpty()){
            removePlayer();
            removeBottomPanel();
        }
        else empty=true;
    }

    bool notEmpty(){
        Scalar sums = sum(fgmask);
        if ((sums.val[0] > 10 ) || (sums.val[1] > 10) || (sums.val[2]>10) ) return true;
        return false;
    }


    void difference(Mat &early,Mat &later){
        // POSTERIZE
        early /=100; early *=100;
        later /=100; later *=100;

//        Mat diff = later- early;
//        threshold(diff,fgmask,100,100,CV_THRESH_BINARY);

        fgmask = later - early  ;      //Simple subtraction
    }



    void removePlayer(){
        // REMOVE PLAYER  =  Black out center of screen
        int row_rad = fgmask.rows/2,     col_rad = fgmask.cols/2;
        int block_size_row = row_rad/4,  block_size_col = col_rad/6;
        int offset_X = 0,               offset_Y = -50;

        for (int y= offset_Y + row_rad - block_size_row; y< row_rad + block_size_row; y++)
        {
            for (int x= offset_X + col_rad - block_size_col; x < col_rad + block_size_col; x++) {
                Vec3b &pixels = fgmask.at<Vec3b>(y,x);
                pixels.val[0] = pixels.val[1] = pixels.val[2] = uchar(0);
            }
        }
    }

    void removeBottomPanel(){
        // REMOVE BOTTOM PANEL // remove 1/8th
        for (int y= fgmask.rows-(fgmask.rows/8); y< fgmask.rows; y++)
        {
            for (int x= 0; x < fgmask.cols; x++)
            {
                Vec3b &pixels = fgmask.at<Vec3b>(y,x);
                pixels.val[0] = pixels.val[1] = pixels.val[2] = uchar(0);
            }
        }
    }
};
