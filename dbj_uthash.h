
#pragma once

#ifndef DBJ_EXTERN_C_BEGIN
#error please include through dbj_collector.h
#endif


DBJ_EXTERN_C_BEGIN

// (c) 2021 by dbj@dbj.org
// variants for when unsigned char [] is used
// otherwise compilers will loudly complain
// if one attempts to use HASH_*_STR macros with unsigned char []

#define HASH_FIND_UCHARR(head, findstr, out)                     \
    do {                                                         \
        unsigned _uthash_hfstr_keylen =                          \
            (unsigned)uthash_strlen((const char *)findstr);      \
        HASH_FIND(hh, head, findstr, _uthash_hfstr_keylen, out); \
    } while (0)

#define HASH_ADD_UCHARR(head, strfield, add)                        \
    do {                                                            \
        unsigned _uthash_hastr_keylen =                             \
            (unsigned)uthash_strlen((const char *)(add)->strfield); \
        HASH_ADD(hh, head, strfield[0], _uthash_hastr_keylen, add); \
    } while (0)

#define HASH_REPLACE_UCHARR(head, strfield, add, replaced)             \
    do {                                                               \
        unsigned _uthash_hrstr_keylen =                                \
            (unsigned)uthash_strlen((const char *)(add)->strfield);    \
        HASH_REPLACE(hh, head, strfield[0], _uthash_hrstr_keylen, add, \
                     replaced);                                        \
    } while (0)

/* --------------------------------------------------- */

DBJ_EXTERN_C_END

