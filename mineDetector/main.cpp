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

    Point window_coords(arg->topleft_x, arg->topleft_y);
    Point window_dims(arg->width, arg->height);

    Point player_coords(window_dims.x/2, window_dims.y/2);

    bool xdo = arg->xdo, nearest = arg->nearest, debug = arg->debug;
    int runs = arg->runs;

    if(arg->early.empty()) //Images not given, make
    {
		delete arg;

        Display *display = XOpenDisplay(NULL);
        Window root = DefaultRootWindow(display);

        int run_count = (runs==-1)?INT_MAX:runs;
        uint total_digtime_elapsed = 0;
        uint player_idle_total = 0;

        PlayerFinder *plf;

        int seconds_between_frames=1;

        while(run_count-- >0)
        {
            Mat early, later;

            total_digtime_elapsed += TimeOps::randsleep(1,2); //Initial random wait before acting


            CVFuncs::populateMat(early, display, root,
                                 window_coords.x, window_coords.y,
                                 window_dims.x, window_dims.y);


            TimeOps::sleep(seconds_between_frames * 987543);
            total_digtime_elapsed += seconds_between_frames;

            CVFuncs::populateMat(later, display, root,
                                 window_coords.x, window_coords.y,
                                 window_dims.x, window_dims.y);

            // Check on player movement
            plf = new PlayerFinder(player_coords, early, later, debug);

            // player idle how many times in row?
            if(plf->is_idle) player_idle_total += 1;
            else player_idle_total = 0;


            cerr << "consec_idle: [" << player_idle_total << "%" << player_idle_thresh << "] " << flush;
            cerr << ", dig_time: ["
                 << total_digtime_elapsed/1000000 << " > " << digtime_max_limit/1000000 << "]" << flush;
            cerr << ", movePix: " << plf->move_pix << endl;

            delete plf;

            if ((player_idle_total > player_idle_thresh) || (total_digtime_elapsed > digtime_max_limit) )
            {
                OreFinder *orf = new OreFinder (early, later, debug);
				keyvect ore_locs(orf->ore_locs);
				delete orf;

                if (xdo && ore_locs.size() > 1){
                    DesktopOps::dropOre(window_coords);

                    DesktopOps::clickOnOne(ore_locs, nearest,
                                           window_coords,
                                           player_coords);
                }
                total_digtime_elapsed = 0; //reset count
                player_idle_total = 0;
                cerr << "Digging..." << endl;
            }
        }
//        delete display;
    }
    else {
        Mat early = imread(arg->early), later = imread(arg->later);
        player_coords = Point(early.cols, early.rows);

        PlayerFinder plf(player_coords,early, later, arg->debug);

        delete arg;

        //        OreFinder(early, later, true);
    }
}
