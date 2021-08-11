#ifndef DBJ_COLLECTOR_INC
#define DBJ_COLLECTOR_INC
/*
(c) 2021 by dbj@dbj.org

Collect the pointers to the blocks allocated. Free them on app exit.

NOTE! Must #defin DBJ_COLLECTOR_IMP before this file on exactly one ocassion
*/

#include "../uthash/src/utlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#ifndef __clang__
#ifndef __GNUC__
#error Please use clang or GCC compilers
#endif // __GNUC__
#endif // __clang__

/*-----------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
    /*-----------------------------------------------------------------*/

// the API
struct dbj_collector_;
struct dbj_collector_ *dbj_collector_global = 0;

void *dbj_collector_alloc(size_t);

// TBD: void * dbj_collector_free ( void * );

#ifdef DBJ_COLLECTOR_IMP

typedef struct dbj_collector_
{
    struct dbj_collector_ *next, *prev;
    unsigned char block[];
} dbj_collector_node;

// return 0 on ENOMEM and sets errno to ENOMEM
void *dbj_collector_alloc(size_t size_)
{
    dbj_collector_node *new_node = (dbj_collector_node *)calloc(1, sizeof(dbj_collector_node) + sizeof(unsigned char[size_]));

    if (new_node)
    {
        DL_APPEND(dbj_collector_global, new_node);
        return new_node->block;
    }

    errno = ENOMEM;
    return 0;
}

#if defined(__clang__) || defined(__GNUC__)
#define DBJ_COLLECTOR_DESTRUCTOR __attribute__((destructor))
#elif defined(_MSC_VER)
#define DBJ_COLLECTOR_DESTRUCTOR
#else // _MSC_VER
#error Unknown compiler ...
#endif

DBJ_COLLECTOR_DESTRUCTOR void dbj_collector_destruct(void)
{
    dbj_collector_node *node = 0, *tmp = 0;
    DL_FOREACH_SAFE(dbj_collector_global, node, tmp)
    {
        DL_DELETE(dbj_collector_global, node);
        free(node);
    }

    dbj_collector_global = 0;
}

#undef DBJ_COLLECTOR_DESTRUCTOR

#endif // DBJ_COLLECTOR_IMP

/*-----------------------------------------------------------------*/
#ifdef __cplusplus
} extern "C"
#endif // __cplusplus
    /*-----------------------------------------------------------------*/

#endif // DBJ_COLLECTOR_INC
