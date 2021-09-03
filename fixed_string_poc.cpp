
/*
release build result:

[----------]Running 2 testmarks.
[ RUN      ]test_01.fixed_string
[      OK  ]test_01.fixed_string (mean 0.032us, confidence interval +- 16.793465%)
[ RUN      ]test_01.std_string
[      OK  ]test_01.std_string (mean 0.034us, confidence interval +- 16.073550%)
[----------]2 testmarks ran.
[  PASSED  ]2 testmarks.

WIN10 PRO, 8GB RAM, on SSD

(c) 2021 by dbj@dbj.org
*/
#define STB_DS_IMPLEMENTATION
#include "fixed_string.h"

#define UTEST_IMPLEMENTATION
// see .vscode/c_cpp_properties.json
// see  build-fixed-string-poc.cmd
#include <ubut/utest.h>



// one is perhaps enough
// remember you are NOT supposed to free Fixie data
// it stays in the memory untill the very end
inline auto global_fixie_ = dbj::fixed_string::make(0xFF);

template <typename VECTOR_ , size_t N>
inline auto movein_assign_moveout (VECTOR_ fixie_, const char (&data_)[N] )
{
  return fixie_.assign(data_);
};

UTEST(test_02, hammer_the_fixed_string)
{
  global_fixie_ = movein_assign_moveout(global_fixie_, "STRING LITERAL");

  auto local_pixie_ = global_fixie_.clean('?') ;

  /*
  just a view, therefore data inside is cleaned
  and both are referencing the same cleaned data
  */
  EXPECT_TRUE( global_fixie_[0] == '?' ) ;
  EXPECT_TRUE( local_pixie_[0] == '?' ) ;

  bool e1 = global_fixie_.empty() ;
  bool e2 = local_pixie_.empty() ;

  EXPECT_TRUE( e1 == e2 );

}

// but this one will be compared with the one bellow it
// the test idea itself is nicked from here:
// https://en.cppreference.com/w/cpp/string/basic_string_view/find_last_of
UTEST(test_01, fixed_string)
{
  // NOTE: ""sv is not user definable, ""_sv is
  using namespace nonstd::literals;
  constexpr auto N = nonstd::string_view::npos;

  bool all_ok = false;

  if (
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
      N == "namespace"_sv.find_last_of("cdef", 3 /* pos [0..3]: "name" */, 2 /* "cd" */))
    all_ok = true;
        EXPECT_TRUE(all_ok);
}

#include <string_view>
UTEST(test_01, std_string)
{
  using namespace std::literals;
  using namespace std::string_view_literals;
  constexpr auto N = std::string_view::npos;

  bool all_ok = false;

  if (
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
      N == "namespace"sv.find_last_of("cdef", 3 /* pos [0..3]: "name" */, 2 /* "cd" */))
    all_ok = true;
    EXPECT_TRUE(all_ok);
}

UTEST_STATE; // note there is no ()!
int main(int argc, const char *const argv[])
{
  return utest_main(argc, argv);
}

