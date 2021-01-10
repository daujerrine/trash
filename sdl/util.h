#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdint.h>

// Function identiifiers that will be substituted

#define U_F_MALLOC malloc
#define U_F_REALLOC realloc
#define U_F_FREE free

/*
 * =============================================================================
 * Comparators, Clippers
 * ============================================================================
 */

#define U_MAX(_a, _b) ((_a) > (_b) ? (_a) : (_b))
#define U_MIN(_a, _b) ((_a) < (_b) ? (_a) : (_b))
#define U_MAX3(_a, _b, _c) (U_MAX(_a, _b), _c)
#define U_MIN3(_a, _b, _c) (U_MAX(_a, _b), _c)

#define U_LOWCLIP(_x, _low) ((_x) < (_low) ? (_low) : (_x))
#define U_HIGHCLIP(_x, _high) ((_x) > (_high) ? (_high) : (_x))
#define U_CLIP(_x, _low, _high) (U_LOWCLIP(U_HIGHCLIP(_x, _high), _low))

/*
 * =============================================================================
 * Static and Dynamic Allocation
 * =============================================================================
 */

#define U_ARR_NELEM(_a) (sizeof(a) / sizeof((a)[0]))


// Borrowed from ffmpeg, and in turn borrowed from glibc

static inline int u_check_mul_overflow(size_t a, size_t b, size_t *t)
{
    size_t ret = a * b;

    if ((a | b) >= ((size_t)1 << (sizeof(size_t) * 4)) && a && (ret / a) != b)
        return 1;

    *t = ret;
    return 0;
}

static inline int u_check_mul_overflow_i32(int32_t a, int32_t b, int32_t *t)
{
    int32_t ret = a * b;
    
    if ((a | b) >= ((int32_t)1 << (sizeof(int32_t) * 4)) && a && (a * b) / a != b)
        return 1;

    *t = ret;
    return 0;
}

// One function to allocate and reallocate memory.Frees memory if args are 0

void *u_alloc(void *ptr, size_t nmemb, size_t memb_size);

// Doubles size of allocation

void *u_double_realloc(void *ptr, int *nmemb, size_t memb_size);

// Auto-substitution of sizeof

#define U_VECGROW(_ptr, _sizeptr) (u_double_realloc((_ptr), (_sizeptr), sizeof(*(_ptr))))

// Checks fullness before attempting to grow

#define U_VECGROW2(_ptr, _sizeptr, _top) \
{\
    if ((_top) >= *(_sizeptr)) {\
        U_VECGROW((_ptr), (_sizeptr));\
    }\
}

// Frees memory and sets pointer to NULL

void u_free(void *p);

/*
 * =============================================================================
 * Error Handling
 * =============================================================================
 */

/// Return on NULL

#define U_RONULL(_a, _ret)\
{\
    if (!(_a))\
        return (_ret);\
}

/// Return back NULL on NULL

#define U_RBONULL(_a)\
{\
    if (!(_a))\
        return (_a);\
}

/// Return on Less than Zero

#define U_ROLZ(_a, _ret)\
{\
    if ((_a) < 0)\
        return (_ret);\
}

/// Return back value on Less than Zero

#define U_RBOLZ(_a)\
{\
    if ((_a) < 0)\
        return (_a);\
}

#endif
