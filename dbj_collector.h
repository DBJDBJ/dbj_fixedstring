#ifndef DBJ_COLLECTOR_INC_
#define DBJ_COLLECTOR_INC_
/*

(c) 2021 by dbj@dbj.org

Collect *ALL* the blocks allocated. Free them on app exit. *NOT* before.

NOTE! Must #define DBJ_COLLECTOR_IMP before this file on exactly one ocassion
and in exactly one compilation unit. (C or CPP file, not header)

Here you decide at compile time which implementation is desired
please include through here always

*/

#ifdef __clang__
#pragma clang system_header
#else // ! __clang__
#error please use clang or clang-cl.exe as packed with Visual Studio 2019; yes GCC should work :wink:
#endif

#undef DBJ_EXTERN_C_BEGIN
#undef DBJ_EXTERN_C_END

#ifdef __cplusplus                
#define		DBJ_EXTERN_C_BEGIN extern "C" {
#define		DBJ_EXTERN_C_END  }
#else // ! __cplusplus
#define		DBJ_EXTERN_C_BEGIN
#define		DBJ_EXTERN_C_END
#endif // !__cplusplus

#include <crtdbg.h>

#define assert _ASSERTE

#ifdef DBJ_COLLECTOR_INC_USES_VECTOR
#include "dbj_collector_vector_imp.h"
#else  // default is implementation using UTHASH
#include "dbj_collector_hash_imp.h"
#endif  // ! DBJ_COLLECTOR_INC_USES_VECTOR

#endif // DBJ_COLLECTOR_INC_