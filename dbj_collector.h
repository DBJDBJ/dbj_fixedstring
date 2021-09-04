#ifndef DBJ_COLLECTOR_INC
#define DBJ_COLLECTOR_INC
/*
(c) 2021 by dbj@dbj.org

Collect the blocks allocated. Free them on app exit.

NOTE! Must #define DBJ_COLLECTOR_IMP before this file on exactly one ocassion
*/

#ifndef DBJ_EXTERN_C_BEGIN
#include <dbj_capi/ccommon.h>
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

#include "vector/vector.h"

DBJ_EXTERN_C_BEGIN

enum  { DBJ_COLLECTOR_INIT_SZE = 0xFF ,DBJ_COLLECTOR_INIT_CCTY = DBJ_COLLECTOR_INIT_SZE * 2 } ;

typedef struct dbj_collector_
{
    unsigned char block[];
} dbj_collector_node;

// not thread safe. yet.
 dbj_vector_t *dbj_collector_vector (void) {
         static dbj_vector_t * vector_ = create_vector( DBJ_COLLECTOR_INIT_SZE, DBJ_COLLECTOR_INIT_CCTY) ;
         DBJ_ASSERT(vector_);
    return vector_ ;
}

//  on ENOMEM sets errno to ENOMEM and returns 0
void *dbj_collector_alloc(size_t size_)
{
    dbj_collector_node *new_node = 
    (dbj_collector_node *)calloc(1, sizeof(dbj_collector_node) + sizeof(unsigned char[size_]));

    if (new_node)
    {
                    #ifndef NDEBUG
            int rez =
            #endif
        vector_push(dbj_collector_vector(), new_node);
        DBJ_ASSERT(DBJ_VCTR_IS_SUCCESS(rez)) ;
        return new_node->block;
    }

    errno = ENOMEM;
    return 0;
}

// Again: use with caution!
bool dbj_collector_free ( void * block_pointer )
{
    if (! block_pointer ) return false ;

    const size_t size_ = vector_size( dbj_collector_vector() );
    dbj_collector_node *node_ = 0 ;
    for( size_t k = 0; k < size_; ++k )
    {
        node_ = (dbj_collector_node *) vector_at( dbj_collector_vector(), k );
        DBJ_ASSERT(node_);
        if( (void *)node_->block == block_pointer )
        {
            #ifndef NDEBUG
            int rez =
            #endif
            vector_erase(dbj_collector_vector(), k);
            DBJ_ASSERT(DBJ_VCTR_IS_SUCCESS(rez)) ;
            free(node_);
        return true ;
        }
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
    const size_t size_ = vector_size( dbj_collector_vector() );
    dbj_collector_node *node_ = 0 ;
    for( size_t k = 0; k < size_; ++k )
    {
        node_ = (dbj_collector_node *) vector_at( dbj_collector_vector(), k );
        DBJ_ASSERT(node_);
            #ifndef NDEBUG
            int rez =
            #endif
            vector_erase(dbj_collector_vector(), k);
            DBJ_ASSERT(DBJ_VCTR_IS_SUCCESS(rez)) ;
            free(node_);
    }
    vector_clear(dbj_collector_vector()) ;
}

#undef DBJ_COLLECTOR_DESTRUCTOR

DBJ_EXTERN_C_END

#endif // DBJ_COLLECTOR_IMP
////////////////////////////////////////////////////////////////////////////////////



#endif // DBJ_COLLECTOR_INC