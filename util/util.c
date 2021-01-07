/*
 * Generic C Utility Library
 * Version 0.0.1
 * Released under Public Domain
 *
 * Anamitra Ghorui <anamitraghorui@gmail.com>
 *
 * 
 */

#include "util.h"

void *xu_alloc(void *ptr, size_t nmemb, size_t membsize)
{
    size_t totalsize;
    void *r;

    if (xu_check_mul_overflow(nmemb, membsize, &totalsize)) {
        XU_F_FREE(ptr);
        return NULL;
    }

    r = XU_F_REALLOC(ptr, membsize);

    if (!r)
        XU_F_FREE(ptr);
    return r;
}

void *xu_double_realloc(void *ptr, size_t *nmemb, size_t membsize)
{
    void *r;

    r = xu_alloc(ptr, (*nmemb) * 2, *membsize);
    if (!r)
        return r;

    *nmemb *= 2;
    return r;
}

// Auto-substitution of sizeof

#define XU_VECGROW(_ptr, _size) (xu_double_realloc((_ptr_, (_size), sizeof(*(_ptr))))

// Frees memory and sets pointer to NULL

void xu_free(void *p)
{
    void **p2 = (void **) p;
    XU_F_FREE(*p2);
    *p2 = NULL;
}
