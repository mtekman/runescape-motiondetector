#include "args.h"
#include "desktopops.h"
#include "orefinder.h"
#include "playerfinder.h"

int main(int argc, char ** argv)
{
    Args arg = Args(argc,argv);

    uint digtime_max_limit = arg.maxdig;
    uint player_idle_thresh = 3;

    PlayerFinder *plf;

    if (arg.init())
    {
        int seconds_between_frames = 1;
        int run_count = (arg.runs==-1)?INT_MAX:arg.runs;

        uint total_digtime_elapsed = 0;
        uint player_idle_total = 0;

        while(run_count-- >0)
        {
            Mat early, later;
            total_digtime_elapsed += TimeOps::randsleep(1,2); //Initial random wait before acting

            DesktopOps::populateMat(early);

            TimeOps::sleep(seconds_between_frames * 1987543);
            total_digtime_elapsed += seconds_between_frames;

            DesktopOps::populateMat(later);

            // Check on player movement
            plf = new PlayerFinder(
                        DesktopOps::player_coords,
                        early, later,
                        arg.debug);

            // player idle how many times in row?
            if(plf->is_idle) player_idle_total += 1;
            else player_idle_total = 0;


            cerr << "consec_idle: [" << player_idle_total << "%" << player_idle_thresh << "] " << flush;
            cerr << ", dig_time: ["
                 << total_digtime_elapsed/1000000 << ">" << digtime_max_limit/1000000 << "]" << flush;
            cerr << ", movePix: " << plf->move_pix << endl;

            delete plf;

            if (    (player_idle_total > player_idle_thresh)
                    ||
                    (total_digtime_elapsed > digtime_max_limit) )
            {
                OreFinder *orf = new OreFinder (
                            early, later,
                            arg.debug);

                keyvect ore_locs(orf->ore_locs);
				delete orf;

                if (arg.xdo && ore_locs.size() > 1){

                    DesktopOps::clickOnOne(
                                ore_locs,
                                arg.nearest);
                    //Reset
                    total_digtime_elapsed =0;
                    player_idle_total = 0;
                    cerr << "Digging..." << endl;
                }

            } else {
                // Player is now assumedly digging, while doing so -- check for completion

                if (DesktopOps::pickSignUp()){
                    //                    //If found more than once consecutivey
                    //                    //then set player_consecutive_idle_status beyond thresh.

                    //                    if (++pick_sign_consec>=1){
                    player_idle_total = player_idle_thresh + 1;
                    //                        pick_sign_consec = 0;

                    //Drop ore after digging
                    cerr << "Found Pick sign, dropping ore "<< endl;
                    DesktopOps::dropOre();
                    DesktopOps::dropOre();
                }
            }
        }
        return 0;
    }

    // ------------------------------------------------------- //
    // Playground Area
    // ------------------------------------------------------- //

    Mat early = imread(arg.early), later = imread(arg.later);

    cvtColor(early,early,CV_BGR2HSV);
    cvtColor(later, later, CV_BGR2HSV);

    //        PlayerFinder (player_coords, early, later, arg.debug);
    //        OreFinder(early, later, arg.debug);

    Mat *prt = new Mat(early);

    // Test pick:
    //    cout << "Pick_sign: " << DesktopOps::pickSignUp(prt) << endl;

    // Test drop:
    Point drop = DesktopOps::findOre(false, 3, prt);
    cout << "Drop sign: " << drop.x << "," << drop.y << endl;

    delete prt;
    return 0;
}
