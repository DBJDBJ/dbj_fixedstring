/*
(c) 2021 by dbj@dbj.org
*/
#include "fixed_string.h"
#include "ubut/ubench.h"

UBENCH ( bench_01 , fixed_string )
{
    // NOTE: ""sv is not user definable
    using namespace nonstd::literals;
  constexpr auto N = nonstd::string_view::npos;
 
  assert(
    5 == "delete"_sv.find_last_of("cdef"_sv) &&
      //       └────────────────────┘ 
    N == "double"_sv.find_last_of("fghi"_sv) &&
      //
    0 == "else"_sv.find_last_of("bcde"_sv, 2 /* pos [0..2]: "els" */) &&
      //  └────────────────────────┘
    N == "explicit"_sv.find_last_of("abcd"_sv, 4 /* pos [0..4]: "expli" */) &&
      //
    3 == "extern"_sv.find_last_of('e') &&
      //     └────────────────────┘
    N == "false"_sv.find_last_of('x') &&
      //
    0 == "inline"_sv.find_last_of('i', 2 /* pos [0..2]: "inl" */) &&
      //  └───────────────────────┘
    N == "mutable"_sv.find_last_of('a', 2 /* pos [0..2]: "mut" */) &&
      //
    3 == "namespace"_sv.find_last_of("cdef", 3 /* pos [0..3]: "name" */, 3 /* "cde" */) &&
      //     └─────────────────────────┘
    N == "namespace"_sv.find_last_of("cdef", 3 /* pos [0..3]: "name" */, 2 /* "cd" */)
  );

}

#include <string_view>
// https://en.cppreference.com/w/cpp/string/basic_string_view/find_last_of
UBENCH ( bench_01, std_string )
{
   using namespace std::string_view_literals;
  constexpr auto N = std::string_view::npos;
 
  assert(
    5 == "delete"sv.find_last_of("cdef"sv) &&
      //       └────────────────────┘
    N == "double"sv.find_last_of("fghi"sv) &&
      //
    0 == "else"sv.find_last_of("bcde"sv, 2 /* pos [0..2]: "els" */) &&
      //  └────────────────────────┘
    N == "explicit"sv.find_last_of("abcd"sv, 4 /* pos [0..4]: "expli" */) &&
      //
    3 == "extern"sv.find_last_of('e') &&
      //     └────────────────────┘
    N == "false"sv.find_last_of('x') &&
      //
    0 == "inline"sv.find_last_of('i', 2 /* pos [0..2]: "inl" */) &&
      //  └───────────────────────┘
    N == "mutable"sv.find_last_of('a', 2 /* pos [0..2]: "mut" */) &&
      //
    3 == "namespace"sv.find_last_of("cdef", 3 /* pos [0..3]: "name" */, 3 /* "cde" */) &&
      //     └─────────────────────────┘
    N == "namespace"sv.find_last_of("cdef", 3 /* pos [0..3]: "name" */, 2 /* "cd" */)
  );

}


  UBENCH_STATE; // note there is no ()!  
  int main(int argc, const char *const argv[]) { 
    return ubench_main(argc, argv);      
  }
