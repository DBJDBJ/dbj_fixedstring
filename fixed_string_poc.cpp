
/*
~/out/comp --confidence=20

[----------]Running 2 benchmarks.
[ RUN      ]test_01.dbj_fixed_string
[      OK  ]test_01.dbj_fixed_string (mean 0.183us, confidence interval +- 6.269403%)
[ RUN      ]test_01.std_string_view
[      OK  ]test_01.std_string_view (mean 0.034us, confidence interval +- 18.069525%)
[----------]2 benchmarks ran.
[  PASSED  ]2 benchmarks.

WIN10 PRO, 8GB RAM, on SSD

(c) 2021 by dbj@dbj.org
*/

#define DBJ_COLLECTOR_IMP 
// #define STB_DS_IMPLEMENTATION
#include "fixed_string.h"

#define UBENCH_IMPLEMENTATION
// see .vscode/c_cpp_properties.json
// see  build-fixed-string-poc.cmd
#include "ubut/ubench.h"


// unlike std string view we do not loose the slab we are pointing to
// and that slab is autofreed on app exit
template <typename DBJ_VCTR_ , size_t N>
inline auto movein_assign_moveout (DBJ_VCTR_ fixie_, const char (&data_)[N] )
{
  // since all of them are now using the same char slab this value return is 
  // superficial, the slabe changes are visible for all of them views
  // view-ing it
  return fixie_.assign(data_);
};

// irelevant for benchmarking because we do not have std:: variant 
static void hammer_the_fixed_string (void)
{

// remember you are NOT supposed to free Fixie data
// it stays in the memory untill the very end
// thus on each call to make one new  char[] will be allocated
// if that is what you really want to do
auto global_fixie_ = dbj::fixed_string::make(0xFF);

 // now local and global are pointing to the same char[]
  auto local_pixie_ = movein_assign_moveout(global_fixie_, "STRING LITERAL");
// and now both will be stamped over with '?'
  global_fixie_.clean('?') ;

  /*
  just a view, therefore data inside is cleaned for both
  as both are referencing the same cleaned data
  */
  assert( global_fixie_[0] == '?' );
  assert( local_pixie_[0] == '?' );
  assert (global_fixie_.empty() == local_pixie_.empty() );

}

// but this one will be compared with the one bellow it
// the test idea itself is nicked from here:
// https://en.cppreference.com/w/cpp/string/basic_string_view/find_last_of
//
UBENCH(test_01, dbj_fixed_string)
{
  // NOTE: ""sv is not user definable, ""_sv is
  using namespace nonstd::literals;
  constexpr auto N = nonstd::string_view::npos;

  bool all_ok = 
    (
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
    ) ;
        // EXPECT_TRUE(all_ok);
}

#include <string_view>
UBENCH(test_01, std_string_view)
{
  using namespace std::literals;
  using namespace std::string_view_literals;
  constexpr auto N = std::string_view::npos;

  bool all_ok = (
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
    // EXPECT_TRUE(all_ok);
}

UBENCH_STATE; // note there is no ()!
int main(int argc, const char *const argv[])
{
  #ifdef _MSC_VER
  // only for windows and only for windows cmd.exe
  // kick start the VT100 emulator
        system(" ");
  #endif

  hammer_the_fixed_string();

  return ubench_main(argc, argv);
}

