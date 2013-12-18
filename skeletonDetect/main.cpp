#define DISPLAY  //imshow prompts

#define neigh_size 2

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <X11/Xlib.h>

#include "imagediff.h"

using namespace cv;
using namespace std;

void usage(){
    fprintf(stderr, "Prints X,Y coordinate of largest detected motion between either between two images or a window position taken at 1/4 second intervals\n\n");
    fprintf(stderr, "usage: skeletonDetect <first.jpg> <second.jpg> [--debug]");
    fprintf(stderr, "\n       or");
    fprintf(stderr, "\n       skeletonDetect <topleft_X> <topleft_Y> <width> <height> [--debug]\n");
    exit(-1);
}

class Args{
public:
    string early, later;
    int topleft_x, topleft_y, width, height;
    bool debug;

    Args(int argc, char ** argv)
    {
        topleft_x = topleft_y = width = height = -1;
        debug = false;

        switch(argc){
        case 0:
        case 1:
        case 2: usage(); break;
            //image no debug flag
        case 3:
            early = argv[1]; later= argv[2]; break;
            //image w/ debug
        case 4:
            early = argv[1]; later= argv[2]; debug=true; break;
            //positions no debug flag
        case 5:
            topleft_x = atoi(argv[1]), topleft_y = atoi(argv[2]), width = atoi(argv[3]), height = atoi(argv[4]);
            break;
            // THIS NEEDS TO REPEAT UNTIL TERMINATED!
          case 6:
            //positions with debug flag
            debug = true;
            topleft_x = atoi(argv[1]), topleft_y = atoi(argv[2]), width = atoi(argv[3]), height = atoi(argv[4]);
            break;
            // THIS NEEDS TO REPEAT UNTIL TERMINATED!
        default:
            usage(); break;
        }
    }
};


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

#ifdef DISPLAY
    imshow("Test",img);
    waitKey(0);
#endif

    return 0;
}


//////////////////////////////////////////////////////

int main(int argc, char ** argv)
{   
    Detector *det = 0;
    Args *arg = new Args(argc,argv);

    if(arg->early.empty())
    {
        Display *display = XOpenDisplay(NULL);
        Window root = DefaultRootWindow(display);

        while(true){
            Mat early, later;
            populateMat(early, display, root,
                        arg->topleft_x,arg->topleft_y,
                        arg->width,arg->height);
            sleep(0.5);
            populateMat(later, display, root,
                        arg->topleft_x,arg->topleft_y,
                        arg->width,arg->height);


            det = new Detector(arg->debug, early, later);
            delete det;
        }
    }
    else {
        Mat early = imread(arg->early), later = imread(arg->later);
        det = new Detector(arg->debug, early, later);
    }

    delete det;
}





