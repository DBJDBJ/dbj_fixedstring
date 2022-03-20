#ifndef DBJ_COLLECTOR_HASH_INC
#define DBJ_COLLECTOR_HASH_INC
/*
(c) 2021 by dbj@dbj.org

Collect the blocks allocated. Free them on app exit.

Implemented by using UTHASH  https://troydhanson.github.io/uthash/

*/

#ifndef DBJ_EXTERN_C_BEGIN
#error please include through dbj_collector.h
#endif


DBJ_EXTERN_C_BEGIN

// the API
struct dbj_collector_;

void *dbj_collector_alloc(size_t);

// BEWARE: this frees all the autoblocks allocated
// this is destructor called at app exit
void dbj_collector_destruct(void);

// find a node and delete it
// if not found return false
// true otherwise
// CaveatEmptor: this is very dangerous as there can be many 
//               holders of this pointer
//               Use with extreme caution!
bool dbj_collector_free ( void * );

DBJ_EXTERN_C_END

////////////////////////////////////////////////////////////////////////////////////
#ifdef DBJ_COLLECTOR_IMP

#include "../uthash_local/src/uthash.h"

DBJ_EXTERN_C_BEGIN

typedef struct dbj_collector_
{
    /* makes this structure hashable */
    UT_hash_handle hh; 
    /* we'll use this field as the key */
    unsigned char block[]; 
} dbj_collector_node;

// not thread safe. yet.
 dbj_collector_node ** dbj_collector_ptr (void) {
    static dbj_collector_node * hasht_global_pointer_ = NULL ;
    return & hasht_global_pointer_ ;
}

//  on ENOMEM sets errno to ENOMEM and returns 0
void *dbj_collector_alloc(size_t size_)
{
    dbj_collector_node *new_node = 
    (dbj_collector_node *)calloc(1, sizeof(dbj_collector_node) + sizeof(unsigned char[size_]));

    if (new_node)
    {
        dbj_collector_node ** head_ = dbj_collector_ptr();
        HASH_ADD_UCHARR( *head_ , block, new_node);
        assert(*head_);
        return new_node->block;
    }

    errno = ENOMEM;
    return 0;
}

// Again: use with caution!
bool dbj_collector_free ( void * block_pointer )
{
    if (! block_pointer ) return false ;

    dbj_collector_node ** head_ = dbj_collector_ptr();

    if (! (*head_) ) return false ;
    
    dbj_collector_node *node_ = 0 ;

    HASH_FIND_UCHARR( *head_, block_pointer, node_ );

    if ( node_ ){
            HASH_DEL( *head_, node_ );
                block_pointer = NULL ;
        return true ;
    }
        return false ;
}

#if defined(__clang__) || defined(__GNUC__)
#define DBJ_COLLECTOR_DESTRUCTOR __attribute__((destructor))
#elif defined(_MSC_VER)
#define DBJ_COLLECTOR_DESTRUCTOR
#pragma message("Please make sure dbj_collector_destruct() is called on exit.")
#else // _MSC_VER
#error Unknown compiler ...
#endif

DBJ_COLLECTOR_DESTRUCTOR void dbj_collector_destruct(void)
{
     dbj_collector_node ** head_ = dbj_collector_ptr();
    if (*head_){
/* 
If you only want to delete all the items, but not free them 
or do any per-element clean up, 
you can do this more efficiently in a single operation:
*/
        HASH_CLEAR(hh, *head_);
    }
}

#undef DBJ_COLLECTOR_DESTRUCTOR

DBJ_EXTERN_C_END

#endif // DBJ_COLLECTOR_IMP
////////////////////////////////////////////////////////////////////////////////////



#endif // DBJ_COLLECTOR_HASH_INC