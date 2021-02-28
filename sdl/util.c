/*
 * Generic C Utility Library
 * Version 0.0.1
 * Released under Public Domain
 * 
 */

#include "util.h"

void *u_alloc(void *ptr, size_t nmemb, size_t memb_size)
{
    size_t total_size;
    void *r;

    if (u_check_mul_overflow(nmemb, memb_size, &total_size)) {
        U_F_FREE(ptr);
        return NULL;
    }

    r = U_F_REALLOC(ptr, total_size);

    if (!r)
        U_F_FREE(ptr);
    return r;
}

void *u_double_realloc(void *ptr, int *nmemb, size_t memb_size)
{
    void *r;

    r = u_alloc(ptr, (*nmemb) * 2, memb_size);
    if (!r)
        return r;

    *nmemb *= 2;
    return r;
}

// Frees memory and sets pointer to NULL

void u_free(void *p)
{
    void **p2 = (void **) p;
    U_F_FREE(*p2);
    *p2 = NULL;
}
