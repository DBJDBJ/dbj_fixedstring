#ifndef DBJ_FIXED_STRING_H
#define DBJ_FIXED_STRING_H

/*
(c) 2021 dbj@dbj.org

Idea is to implement fixed size string as string_view that can mutate the heap allocated char slab is it refering to.

NOTE: what I do not do is inherit for implementation. Which I have done here :wink:

*/

#define DBJ_COLLECTOR_IMP
#include "dbj_collector.h"

// DBJ: "regulated" #define nssv_USES_STD_STRING_VIEW 0 inside
#define nssv_CONFIG_NO_EXCEPTIONS 1
#define nssv_CONFIG_NO_STREAM_INSERTION 1
#include "string_view_stand_alone.h"

namespace dbj
{

    struct fixed_string final : public nonstd::string_view
    {

        using type = fixed_string;
        using parent_type = nonstd::string_view;
        using value_type = typename parent_type::value_type;

        fixed_string(char *slab_, size_type count_ ) noexcept : parent_type(slab_, count_) {}

        typename parent_type::reference operator[](unsigned idx_) noexcept
        {
            return const_cast<parent_type::reference>(this->parent_type::operator[](size_t(idx_)));
        }

        typename parent_type::reference at(unsigned idx_) noexcept
        {
            return const_cast<parent_type::reference>(this->parent_type::at(size_t(idx_)));
        }

        typename parent_type::pointer data() noexcept
        {
            return const_cast<parent_type::pointer>(this->parent_type::data());
        }

        static type make(unsigned size_) noexcept
        {
            return type((char *)dbj_collector_alloc(size_), size_);
        }

    }; // fixed_string

} // dbj NS

#endif // DBJ_FIXED_STRING_H