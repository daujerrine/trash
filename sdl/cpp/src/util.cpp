/**
 * @file Utility and debugging functions
 *
 */

#ifndef UTIL_H
#define UTIL_H

#include "config.h"

namespace media {

namespace util {

/**
 * Aligns an outer rectangle with an inner one.
 */

static inline Rect rect_align(Rect out, Rect in, Gravity g, int hpad, int vpad)
{
    switch (g) {
    case TOPLEFT:
        return (Rect) { out.x + hpad, out.y + vpad, in.w, in.h };

    case TOP:
        return (Rect) { out.x + out.w / 2 - in.w / 2, out.y + vpad, in.w, in.h };

    case TOPRIGHT:
        return (Rect) { out.x + out.w - in.w - hpad, out.y + vpad, in.w, in.h};

    case RIGHT:
        return (Rect) { out.x + out.w - in.w - hpad, out.y + out.h / 2 - in.h / 2, in.w, in.h };

    case BOTTOMRIGHT:
        return (Rect) { out.x + out.w - in.w - hpad, out.y + out.h - in.h - vpad, in.w, in.h };

    case BOTTOM:
        return (Rect) { out.x + out.w / 2 - in.w / 2, out.y + out.h - in.h - hpad, in.w, in.h };

    case BOTTOMLEFT:
        return (Rect) { out.x + hpad, out.y + out.h - in.h - hpad, in.w, in.h };

    case LEFT:
        return (Rect) { out.x + hpad, out.y + out.h / 2 - in.h / 2, in.w, in.h };

    case CENTER:
        return (Rect) { out.x + out.w / 2 - in.w / 2, out.y + out.h / 2 - 1 - in.h / 2, in.w, in.h };

    default:
        return (Rect) {0, 0, 0, 0};
    }
}


static inline bool point_in_rect(int x, int y, Rect rect)
{
    return ((x) >= (rect).x && (y) >= (rect).y &&
            (x) <= (rect).x + (rect).w && (y) <= (rect).y + (rect).h);
}

};

};

#endif
