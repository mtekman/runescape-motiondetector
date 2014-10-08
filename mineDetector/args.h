#ifndef ARGS_H
#define ARGS_H

#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define VER "v0.1"

//Opts
string debug_arg="--debug";
string xdo_arg="--xdo";
string runs_arg="--runs=";
string near_arg="--nearest";
string maxdig_arg="--maxdig=";

void usage(){
    fprintf(stderr, "Mine Detector %s\n", VER);
    fprintf(stderr, "\nDetecs runescape rocks and looks for twinkle points, returning coordinates\n\n");
    fprintf(stderr, "usage: mineDetector <first.jpg> <second.jpg> [OPTIONS]");
    fprintf(stderr, "\n       or");
    fprintf(stderr, "\n      mineDetector <+X> <+Y> <width> <height> [OPTIONS]\n");
    fprintf(stderr, "\nOPTIONS:");
    fprintf(stderr, "\n  %s\tShow stats of each detection", debug_arg.c_str());
    fprintf(stderr, "\n  %s  \tclick on detection", xdo_arg.c_str());
    fprintf(stderr, "\n  %s\tselect nearest mine instead of random", near_arg.c_str());
    fprintf(stderr, "\n  %sN\tmaximum time to dig before forcing recheck [30]", maxdig_arg.c_str());
    fprintf(stderr, "\n  %sN\tnumber of times to run detector [inf]\n", runs_arg.c_str());
    exit(-1);
}


class Args{
public:
    string early, later;
    int topleft_x, topleft_y, width, height;
    bool debug, xdo, nearest;
    int maxdig, runs;

    Args(int argc, char ** argv)
    {
        if(argc<2) usage();

        topleft_x = topleft_y = width = height = runs = maxdig = -1;
        debug = xdo = nearest = false;

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
                } else if (carg == near_arg){
                    nearest = true;
                    num_hyp_args++;

                } else if (carg.substr(0,7) == runs_arg){
                    int temp_runs = atoi(carg.substr(7,runs_arg.length()-1).c_str());
                    if(temp_runs<1){
                        printf("illegal run value\n",stderr); exit(-1);
                    }
                    runs = temp_runs;
                    num_hyp_args++;
                } else if (carg.substr(0,9) == maxdig_arg){
                    int temp_digs = atoi(carg.substr(9,runs_arg.length()-1).c_str());
                    if(temp_digs<1){
                        printf("illegal dig value\n",stderr); exit(-1);
                    }
                    maxdig = temp_digs;
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

#endif
