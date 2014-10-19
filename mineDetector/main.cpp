#include "args.h"
#include "desktopops.h"
#include "orefinder.h"
#include "playerfinder.h"


int main(int argc, char ** argv)
{   
    Args *arg = new Args(argc,argv);
    uint digtime_max_limit = arg->maxdig;
    uint player_idle_thresh = 3;

    Point DesktopOps::window_coords = Point(arg->topleft_x, arg->topleft_y);
    Point DesktopOps::window_dims = Point(arg->width, arg->height);

    Point player_coords(DesktopOps::window_dims.x/2, DesktopOps::window_dims.y/2);

    bool xdo = arg->xdo, nearest = arg->nearest, debug = arg->debug;
    int runs = arg->runs;

    if(arg->early.empty()) //Images not given, make
    {
		delete arg;

        int run_count = (runs==-1)?INT_MAX:runs;
        uint total_digtime_elapsed = 0;
        uint player_idle_total = 0;

        PlayerFinder *plf;

        int seconds_between_frames=1;

        while(run_count-- >0)
        {
            Mat early, later;

            total_digtime_elapsed += TimeOps::randsleep(1,2); //Initial random wait before acting


            DesktopOps::populateMat(early);

            TimeOps::sleep(seconds_between_frames * 1987543);
            total_digtime_elapsed += seconds_between_frames;

            DesktopOps::populateMat(later);

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
                    DesktopOps::dropOre();

                    DesktopOps::clickOnOne(ore_locs, nearest,player_coords);
                    total_digtime_elapsed = 0; //reset count
                    player_idle_total = 0;
                    cerr << "Digging..." << endl;
                }
            }
        }
//        delete display;
    }
    else {
        Mat early = imread(arg->early), later = imread(arg->later);
        player_coords = Point(early.cols/2, early.rows/2);

        PlayerFinder (player_coords, early, later, debug);
        OreFinder(early, later, arg->debug);
        delete arg;
    }
}
