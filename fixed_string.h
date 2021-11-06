#ifndef DBJ_FIXED_STRING_H
#define DBJ_FIXED_STRING_H

/*
(c) 2021 dbj@dbj.org

Idea is to implement fixed size string as string_view that can mutate the heap allocated char slab is it refering to.

NOTE: what I do not do is inherit for implementation. Which I have done here :wink:

*/

// requires #define DBJ_COLLECTOR_IMP in one compilation unit 
#include "dbj_collector/dbj_collector.h"

// Please see https://github.com/martinmoene/string-view-lite#configuration
// and  string_view.tweak.h

#ifdef __has_include
#if __has_include("string_view_stand_alone/string_view.tweak.h")
#include "string_view_stand_alone/string_view.tweak.h"
#else
#error string_view_stand_alone/string_view.tweak.h missing
#endif
#endif // __has_include

#include "string_view_stand_alone/string_view_stand_alone.h"

namespace dbj
{
            // the only type allowed is char
             // since dbj_collector does only unsigned chars
    class fixed_string final : public nonstd::sv_lite::basic_string_view<char> 
    {
   
    public:
        using type = fixed_string;
        using parent_type = nonstd::sv_lite::basic_string_view<char>;
        using value_type = typename parent_type::value_type;

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

            // extensions start here
            // once more: the only type allowed is char
            // since dbj_collector does only unsigned chars
            // the factory method
        static type make(unsigned size_) noexcept
        {
            return type((char *)dbj_collector_alloc(size_), size_);
        }

        template <size_t N>
        type &assign(const char (&str)[N]) noexcept
        {
            assert(N < this->size());
#ifndef NDEBUG
            int rez =
#endif
                // not parent data()
                memcpy_s(this->type::data(), this->size(), str, N);
            assert(rez == 0);
            return *this;
        }

        // Beware
        //
        // fixed_string is a view to the slab it references
        // it is not an exclusive owner of it, here we clean it and
        // move out a copy of the view to the cleanend data
        // the original will reference the same cleaned data slab
        // as the view returned, much like pointers, but with no pointers.
        type & clean(char const filler_char = char(0)) noexcept
        {
            constexpr auto char_zero = char(0);
            memset(this->type::data(), (filler_char ? filler_char : char_zero), this->size());
            return *this;
        }
        private: 
        // forbiden constructors
                nssv_constexpr fixed_string() nssv_noexcept
            : parent_type()  { }

        fixed_string(char *slab_, size_type count_) noexcept : parent_type(slab_, count_) {}

    }; // fixed_string

} // dbj NS

#endif // DBJ_FIXED_STRING_H