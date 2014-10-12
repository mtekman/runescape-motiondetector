#include "args.h"
#include "orefinder.h"
#include "playerfinder.h"
#include "cvfuncs.h"
#include "desktopops.h"


int main(int argc, char ** argv)
{   
    Args *arg = new Args(argc,argv);
    uint digtime_max_limit = arg->maxdig;
    uint player_idle_thresh = 3;

    Point player_coords;

    if(arg->early.empty()) //Images not given, make
    {
        Display *display = XOpenDisplay(NULL);
        Window root = DefaultRootWindow(display);

        int run_count = (arg->runs==-1)?INT_MAX:arg->runs;
        uint total_digtime_elapsed = 0;
        uint player_idle_total = 0;

        PlayerFinder *plf;

        //Player Coords
        Point center(arg->width/2, arg->height/2);
        Point offset(0,0);

        player_coords = center - offset;


        int seconds_between_frames=1;

        while(run_count-- >0)
        {
            Mat early, later;

            total_digtime_elapsed += TimeOps::randsleep(1,2); //Initial random wait before acting


            CVFuncs::populateMat(early, display, root,
                                 arg->topleft_x,arg->topleft_y,
                                 arg->width,arg->height);


            TimeOps::sleep(seconds_between_frames * 987543);
            total_digtime_elapsed += seconds_between_frames;

            CVFuncs::populateMat(later, display, root,
                                 arg->topleft_x,arg->topleft_y,
                                 arg->width,arg->height);

            // Check on player movement
            plf = new PlayerFinder(player_coords, early, later, arg->debug);

            // player idle how many times in row?
            if(plf->is_idle) player_idle_total += 1;
            else player_idle_total = 0;


            cerr << "consec_idle: [" << player_idle_total << "%" << player_idle_thresh << "] " << flush;
            cerr << ", dig_time: ["
                 << total_digtime_elapsed/1000000 << " > " << digtime_max_limit/1000000 << "]" << flush;
            cerr << ", movePix: " << plf->move_pix << endl;

            delete plf;

            if ((player_idle_total > player_idle_thresh) || (total_digtime_elapsed > digtime_max_limit) ){
                OreFinder orf(early, later, arg->debug);

                if (arg->xdo && orf.ore_locs.size() > 1){
                    Point window_coords(arg->topleft_x, arg->topleft_y);

                    DesktopOps::dropOre(window_coords);

                    DesktopOps::clickOnOne(orf.ore_locs, arg->nearest,
                                           window_coords,
                                           player_coords);

                }

                total_digtime_elapsed = 0; //reset count
                player_idle_total = 0;
                cerr << "Digging..." << endl;
            }
        }
    }
    else {
        Mat early = imread(arg->early), later = imread(arg->later);
        player_coords = Point(early.cols, early.rows);

        PlayerFinder plf(player_coords,early, later, arg->debug);

        //        OreFinder(early, later, true);
    }
}
