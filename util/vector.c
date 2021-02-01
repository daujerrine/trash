#include <stdio.h>
#include <stdlib.h>


#define U_F_MALLOC malloc
#define U_F_REALLOC realloc
#define U_F_FREE free

#define U_STRUCT_MEMB_SIZE(_type, _memb) sizeof(((_type *) 0)->(_memb))


#define VEC_MIN_SIZE 16

#define VEC_DEFINE(_name, _type) \
typedef struct _name { \
    _type  *data; \
    size_t top; \
    size_t size; \
} _name;

static inline int u_check_mul_overflow(size_t a, size_t b, size_t *t)
{
    size_t ret = a * b;

    if ((a | b) >= ((size_t)1 << (sizeof(size_t) * 4)) && a && ret / a != b)
        return 1;

    *t = ret;
    return 0;
}

void *u_alloc(void *ptr, size_t nmemb, size_t membsize)
{
    size_t totalsize;
    void *r;
    if (u_check_mul_overflow(nmemb, membsize, &totalsize)) {
        U_F_FREE(ptr);
        return NULL;
    }
    r = U_F_REALLOC(ptr, totalsize);
    if (!r) {
        U_F_FREE(ptr);
    }
    return r;
}

void *u_double_realloc(void *ptr, size_t *nmemb, size_t membsize)
{
    void *r;

    r = u_alloc(ptr, (*nmemb) * 2, membsize);
    if (!r) {
        return r;
    }
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

// Auto-substitution of sizeof

#define VEC_GROW(_vec) (u_double_realloc((_vec).data, &(_vec).size, sizeof(*((_vec).data))))

#define VEC_INIT(_vec, _ok) \
do { \
    (_vec).data = u_alloc(NULL, VEC_MIN_SIZE, sizeof((_vec).data)); \
    if (!(_vec).data) { \
        (_ok) = 0; \
        break; \
    } \
    (_vec).top  = 0; \
    (_vec).size = VEC_MIN_SIZE; \
    (_ok) = 1; \
} while(0);

#define VEC_PUSH(_vec, _val, _ok) \
do { \
    if ((_vec).top >= (_vec).size) { \
        if (!VEC_GROW((_vec))) { \
            (_ok) = 0; \
            break; \
        } \
    } \
    (_vec).data[(_vec).top++] = (_val); \
    ok = 1; \
} while (0);

#define VEC_POP(_vec, _val, _ok) \
do { \
    if ((_vec).top == 0) { \
        (_ok) = 0; \
    } \
    (_vec).data[(_vec).top] = (_val); \
    (_vec).top--; \
    ok = 1; \
} while(0);

#define VEC_GET(_vec, _index) ((_vec).data[(_index)])

#define VEC_FREE(_vec) (U_F_FREE((_vec).data))

VEC_DEFINE(IntVec, int);

int main() {
    IntVec k;
    int ok = 0;

    VEC_INIT(k, ok);
    printf("ok = %d\n", ok);
    if (!ok) {
        printf("error\n");
        return 1;
    }

    for (int i = 0; i < 32; i++)
        VEC_PUSH(k, 5, ok);
    printf("%d\n", VEC_GET(k, 0));
    VEC_FREE(k);
    return 0;
}
