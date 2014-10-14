#ifndef TIMEOPS_H
#define TIMEOPS_H

#include <time.h>

struct TimeOps{
    static inline void sleep(unsigned int mseconds)
    {
        clock_t goal = mseconds + clock();
        while (goal > clock());
    }

    inline static uint randsleep(uint min_secs=0, uint max_secs=500){
        min_secs *=1000000;
        max_secs *=1000000;
        uint sleeper = (rand()+min_secs) % max_secs;
        sleep(sleeper);
        return sleeper;
    }
};

#endif
