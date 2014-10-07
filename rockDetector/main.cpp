#include "imagediff.h"
#include "xdesktop.h"

using namespace cv;
using namespace std;

#define VER "v0.1"

//Opts
string debug_arg="--debug";
string xdo_arg="--xdo";
string runs_arg="--runs=";

void usage(){
    fprintf(stderr, "Rock Detector %s\n", VER);
    fprintf(stderr, "\nDetecs runescape rocks and looks for twinkle points, returning coordinates\n\n");
    fprintf(stderr, "usage: rockDetector <first.jpg> <second.jpg> [OPTIONS]");
    fprintf(stderr, "\n       or");
    fprintf(stderr, "\n       rockDetector <+X> <+Y> <width> <height> [OPTIONS]\n");
    fprintf(stderr, "\nOPTIONS:");
    fprintf(stderr, "\n  %s\tShow stats of each detection", debug_arg.c_str());
    fprintf(stderr, "\n  %s  \tclick on detection", xdo_arg.c_str());
    fprintf(stderr, "\n  %sN\tnumber of times to run detector [infinite]\n", runs_arg.c_str());
    exit(-1);
}


/* Rock detection criteria:
 *      Detect rocks in first and second image.
 *          Check positions persist in both and filter out blobs that dont
 *
 *      From available rock blob locations, detect twinkle in image diff
 *
 * Man detection criteria:
 *      Motion at center over N frames == digging
 *
 *
 * Checks every second.
 * if motion is detected:
 *      if man is digging:
 *          ignore targets
 *      else:
 *          Pick one of available targets
 */



class Args{
public:
    string early, later;
    int topleft_x, topleft_y, width, height, runs;
    bool debug, xdo;

    Args(int argc, char ** argv)
    {
        if(argc<2) usage();

        topleft_x = topleft_y = width = height = runs = -1;
        debug = xdo = false;

        //Determine number of hypen arguments
        int num_hyp_args= 0;
        int num_oth_args= argc-1;

        for (int i=argc-1; i>0; i--){
            string carg = argv[i];
            if (carg[0]=='-'){
                if (carg == debug_arg){
                    debug=true;
                    num_hyp_args++;
                } else if (carg == xdo_arg){
                    xdo = true;
                    num_hyp_args++;
                } else {
                    fprintf(stderr,"cannot parse:%s\n", carg.c_str());
                    exit(-1);
                }
            }
        }
        num_oth_args -= num_hyp_args;

        switch(num_oth_args){
        case 2:
            early=argv[1]; later=argv[2]; break;
        case 4:
            topleft_x = atoi(argv[1]), topleft_y = atoi(argv[2]), width = atoi(argv[3]), height = atoi(argv[4]);
            break;
        default:
            printf("Wrong number of arguments!\n",stderr); exit(-1);
        }

//        fprintf(stderr,"num_hyp=%d  num_oth=%d, debug=%d  xdo=%d, runs=%d\n",num_hyp_args, num_oth_args, debug, xdo, runs);
//        exit(-1);

    }
};


//////////////////////////////////////////////////////

int main(int argc, char ** argv)
{   
    RockDetector *det = 0;
    Args *arg = new Args(argc,argv);

    bool single_run = arg->runs < 2;

    if(arg->early.empty())
    {
        Display *display = XOpenDisplay(NULL);
        Window root = DefaultRootWindow(display);

        int run_count = (arg->runs==-1)?INT_MAX:arg->runs;
        while(run_count-->0){
            Mat early, later;
            populateMat(early, display, root,
                        arg->topleft_x,arg->topleft_y,
                        arg->width,arg->height,
                        arg->debug);
            randsleep(1000,3000);
            populateMat(later, display, root,
                        arg->topleft_x,arg->topleft_y,
                        arg->width,arg->height,
                        arg->debug);

            det = new RockDetector(arg->debug, early, later, single_run);
            if (arg->xdo && det->k_index_max!=-1)
            {
                clickhere(arg->topleft_x, arg->topleft_y,int(det->max.pt.x),int(det->max.pt.y));
                typeHits();
            }
            delete det;
        }
    }
    else {
        Mat early = imread(arg->early), later = imread(arg->later);
        det = new RockDetector(arg->debug, early, later, single_run);
    }
//    delete det;
}
