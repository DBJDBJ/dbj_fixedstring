
#define UTEST_IMPLEMENTATION
#include "../ubut/utest.h"

#define DBJ_COLLECTOR_IMP
#include "dbj_collector.h"

UTEST( dbj_collector, basic_test)
{
        void * p_ = dbj_collector_alloc(0xFF);
        EXPECT_NE(p_, NULL);

        EXPECT_TRUE( dbj_collector_free(p_) ) ;

        dbj_collector_destruct();
}


UTEST_STATE;   // <-- note there is no ()! 
  int main(int argc, const char *const argv[]) 
  {
	return utest_main(argc, argv);          
  }