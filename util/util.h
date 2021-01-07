/*
 * Generic C Utility Library
 * Version 0.0.1
 * Released under Public Domain
 *
 * Anamitra Ghorui <anamitraghorui@gmail.com>
 *
 * You can change the namespace prefix using simple replacement. For example,
 * in sed:
 *      sed -e 's/XU/<new namespace>/gi' < util.h > util_project.h
 * 
 */

#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include "vector.h"

// Function identiifiers that will be substituted

#define XU_F_MALLOC malloc
#define XU_F_REALLOC realloc
#define XU_F_FREE free

/*
 * =============================================================================
 * Comparators, Clippers
 * ============================================================================
 */

#define XU_MAX(_a, _b) ((_a) > (_b) ? (_a) : (_b))
#define XU_MIN(_a, _b) ((_a) < (_b) ? (_a) : (_b))
#define XU_MAX3(_a, _b, _c) (XU_MAX(_a, _b), _c)
#define XU_MIN3(_a, _b, _c) (XU_MAX(_a, _b), _c)

#define XU_LOWCLIP(_x, _low) ((_x) < (_low) ? (_low) : (_x))
#define XU_HIGHCLIP(_x, _high) ((_x) > (_high) ? (_high) : (_x))
#define XU_CLIP(_x, _low, _high) (XU_LOWCLIP(XU_HIGHCLIP(_x, _high), _low))

/*
 * =============================================================================
 * Static and Dynamic Allocation
 * =============================================================================
 */

#define XU_ARR_NELEM(_a) (sizeof(a) / sizeof((a)[0]))


// Borrowed from ffmpeg, and in turn borrowed from glibc

static inline int xu_check_mul_overflow(size_t a, size_t b, size_t *t)
{
    size_t ret = a * b;

    if ((a | b) >= ((size_t)1 << (sizeof(size_t) * 4)) && a && t / a != b)
        return 1;

    *t = ret;
    return 0;
}

static inline int xu_check_mul_overflow_i32(int32_t a, int32_t b, int32_t *t)
{
    int32_t ret = a * b;
    
    if ((a | b) >= ((int32_t)1 << (sizeof(int32_t) * 4)) && a && (a * b) / a != b)
        return 1;

    *t = ret;
    return 0;
}

// One function to allocate and reallocate memory.Frees memory if args are 0

void *xu_alloc(void *ptr, size_t nmemb, size_t membsize);

// Doubles size of allocation.

void *xu_double_realloc(void *ptr, size_t *nmemb, size_t membsize);

// Auto-substitution of sizeof

#define XU_VECGROW(_ptr, _size) (xu_double_realloc((_ptr_, (_size), sizeof(*(_ptr))))

// Frees memory and sets pointer to NULL

void xu_free(void *p);

/*
 * =============================================================================
 * Error Handling
 * =============================================================================
 */

/// Return on NULL

#define XU_RONULL(_a, _ret)\
{\
    if (!(_a))\
        return (_ret);\
}

/// Return back NULL on NULL

#define XU_RBONULL(_a)\
{\
    if (!(_a))\
        return (_a);\
}

/// Return on Less than Zero

#define XU_ROLZ(_a, _ret)\
{\
    if ((_a) < 0)\
        return (_ret);\
}

/// Return back value on Less than Zero

#define XU_RBOLZ(_a)\
{\
    if ((_a) < 0)\
        return (_a);\
}

#endif
