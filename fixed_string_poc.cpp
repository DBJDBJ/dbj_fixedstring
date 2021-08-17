
/*
release build result:

[----------]Running 2 benchmarks.
[ RUN      ]bench_01.fixed_string
[      OK  ]bench_01.fixed_string (mean 0.032us, confidence interval +- 16.793465%)
[ RUN      ]bench_01.std_string
[      OK  ]bench_01.std_string (mean 0.034us, confidence interval +- 16.073550%)
[----------]2 benchmarks ran.
[  PASSED  ]2 benchmarks.

WIN10 PRO, 8GB RAM, on SSD

(c) 2021 by dbj@dbj.org
*/
#include "fixed_string.h"
#include "ubut/ubench.h"

// you are more than capable to implement this
// auto fixie = assign( fixie, "DATA" ) ;
template <size_t N>
inline dbj::fixed_string assign(dbj::fixed_string fixie, const char (&str)[N]) noexcept
{
  assert(N < fixie.size());
  int rez = memcpy_s(fixie.data(), fixie.size(), str, N);
  assert(rez == 0);
  return fixie;
}

// Behavioral advice
//
// fixed_string is a view to the slab it references
// it is not an exclusive owner of it, here we clean it and 
// move out a copy of the view to the cleanend data
// the original will reference the same cleaned data slab
// as the view returned, much like pointers, but with no pointers.
inline dbj::fixed_string clean(dbj::fixed_string fixie ) noexcept
{
  constexpr auto char_zero = '\0';
  memset(fixie.data(), char_zero, fixie.size());
  return fixie;
}

// one is perhaps enough
// remember you are NOT supposed to free Fixie data
// it stays in the memory untill the very end
inline auto global_fixie_ = dbj::fixed_string::make(0xFF);

template <typename FT_ , size_t N>
inline auto movein_assign_moveout (FT_ fixie_, const char (&data_)[N] )
{
  return assign(fixie_, data_);
};

// remember: we benchmark abnd develop in the same time,
// thus in here we do not compare 
UBENCH(bench_02, hammer_the_fixed_string)
{
  global_fixie_ = movein_assign_moveout(global_fixie_, "STRING LITERAL");

  auto local_pixie_ = clean(global_fixie_) ;

  /*
  just a view, therefore data inside is cleaned
  and both are referencing the same cleaned data
  */
  assert( global_fixie_[0] == '\0' ) ;
  assert( local_pixie_[0] == '\0' ) ;
}

// but this one will be compared with the one bellow it
// the test idea itself is nicked from here:
// https://en.cppreference.com/w/cpp/string/basic_string_view/find_last_of
UBENCH(bench_01, fixed_string)
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
}

#include <string_view>
UBENCH(bench_01, std_string)
{
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
}

UBENCH_STATE; // note there is no ()!
int main(int argc, const char *const argv[])
{
  return ubench_main(argc, argv);
}

