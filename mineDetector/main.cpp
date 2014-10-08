#include "args.h"
#include "orefinder.h"
#include "playerfinder.h"
#include "cvfuncs.h"
#include "desktopops.h"

using namespace cv;
using namespace std;


int main(int argc, char ** argv)
{   
    Args *arg = new Args(argc,argv);
    bool nearest = true; //add to Args later
    int digtime_max_limit = 10000; // Grab from Args later

    if(arg->early.empty()) //Images not given, make
    {
        Display *display = XOpenDisplay(NULL);
        Window root = DefaultRootWindow(display);

        int run_count = (arg->runs==-1)?INT_MAX:arg->runs;
        uint total_digtime_elapsed = 0;

        while(run_count-- >0)
        {
            Mat early, later;
            CVFuncs::populateMat(early, display, root,
                        arg->topleft_x,arg->topleft_y,
                        arg->width,arg->height);

            total_digtime_elapsed += TimeOps::randsleep(1000,3000);

            CVFuncs::populateMat(later, display, root,
                        arg->topleft_x,arg->topleft_y,
                        arg->width,arg->height);

            PlayerFinder *plf = new PlayerFinder(early, later, arg->debug);

            if (plf->is_idle || (total_digtime_elapsed > digtime_max_limit) ){
                OreFinder *orf = new OreFinder(early, later, arg->debug);
                DesktopOps::clickOnOne(orf->ore_locs, nearest);

                total_digtime_elapsed = 0; //reset count
            }
        }
    }
    else {
        Mat early = imread(arg->early), later = imread(arg->later);
        OreFinder(early, later, true);
    }
}
