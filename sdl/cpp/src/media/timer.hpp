#ifndef MEDIA_TIMER_H
#define MEDIA_TIMER_H

#include "common.hpp"

namespace media {

/**
 * Used for timing operations.
 */
struct Timer {
    int64_t time;
    uint32_t duration;

    Timer(uint32_t duration)
    {
        this->duration = duration;
        this->time = duration;
    }

    inline void update(uint32_t delta);
    inline bool done();
    inline uint32_t delay();
};

inline void Timer::update(uint32_t delta)
{
    time -= delta;
}

inline bool Timer::done()
{
    if (time <= 0) {
        time = duration;
        return true;
    } else {
        return false;
    }
}

inline uint32_t Timer::delay()
{
    if (time > 0) {
        return 0;
    } else {
        return duration - time; 
    }
}

};

#endif
