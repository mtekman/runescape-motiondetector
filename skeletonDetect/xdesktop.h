#include <X11/Xlib.h>
#include <time.h>

static inline void sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

inline static void randsleep(uint min_msecs=0, uint max_msecs=500){
    min_msecs *=1000;
    max_msecs *=1000;
    uint sleeper = (rand()+min_msecs) % max_msecs;
    sleep(sleeper);
}


static void charbychar(char * start, int len){

    for (int i=0; i < len; ++i){
        char t = start[i]; char buffer[20];

        sprintf(buffer,"xdotool key %c\n",t);
        randsleep(50,100);

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


static int populateMat(Mat &img, Display *disp, Window &root,
                       int &x, int &y, int &width, int& height, bool &debug)
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
