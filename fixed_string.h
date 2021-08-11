#ifndef DBJ_FIXED_STRING_H
#define DBJ_FIXED_STRING_H

/*
(c) 2021 dbj@dbj.org

Idea is to implement fixed size string as string_view that can mutate the heap allocated char slab is it refering to.

NOTE: what I do not do is inherit for implementation. Which I have done here :wink:

*/

#define DBJ_COLLECTOR_IMP
#include "dbj_collector.h"

// Please see https://github.com/martinmoene/string-view-lite#configuration
// and  string_view.tweak.h

#ifdef __has_include
# if __has_include("string_view.tweak.h")
#  include "string_view.tweak.h"
# else
#  error string_view.tweak.h missing
# endif
#endif // __has_include

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