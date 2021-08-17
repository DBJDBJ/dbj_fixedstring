
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

// fixed_string is a view to the slab it references
// it is not an exclusive owner of it
// here we clean it and move out a copy of the view to the cleanend data
// thus the original will reference the same data slab
// as the view returned
inline dbj::fixed_string clean(dbj::fixed_string fixie ) noexcept
{
  constexpr auto char_zero = '\0';
  memset(fixie.data(), char_zero, fixie.size());
  return fixie;
}

// one is enough
// remember you are NOT supposed to free pixie data!
inline auto global_pixie_ = dbj::fixed_string::make(0xFF);

inline auto moveinmoveout = [](auto pixie)
{
  return assign(global_pixie_, "DATA");
};

UBENCH(bench_02, assing_to_fixed_string)
{
  global_pixie_ = moveinmoveout(global_pixie_);

  auto local_pixie_ = clean(global_pixie_) ;

  /*
  just a view, therefore data inside is cleaned
  */
  assert( global_pixie_[0] == '\0' ) ;
  assert( local_pixie_[0] == '\0' ) ;
}

UBENCH(bench_01, fixed_string)
{
  // NOTE: ""sv is not user definable
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
// https://en.cppreference.com/w/cpp/string/basic_string_view/find_last_of
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