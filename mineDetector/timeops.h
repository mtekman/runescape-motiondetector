#ifndef TIMEOPS_H
#define TIMEOPS_H

#include <time.h>

struct TimeOps{
    static inline void sleep(unsigned int mseconds)
    {
        clock_t goal = mseconds + clock();
        while (goal > clock());
    }

    inline static uint randsleep(uint min_msecs=0, uint max_msecs=500){
        min_msecs *=1000;
        max_msecs *=1000;
        uint sleeper = (rand()+min_msecs) % max_msecs;
        sleep(sleeper);
        return sleeper;
    }
};

#endif
