#pragma once

/*
Mainly nicked from cppreference
*/

namespace dbj {
namespace nonstd {

// Definitions of common types
#ifdef _WIN64
    typedef unsigned __int64 size_t;
    typedef __int64          ptrdiff_t;
    typedef __int64          intptr_t;
#else
    typedef unsigned int     size_t;
    typedef int              ptrdiff_t;
    typedef int              intptr_t;
#endif

    // https://en.cppreference.com/w/cpp/types/integral_constant
    
template<class T, T v>
struct integral_constant {
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant; // using injected-class-name
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; } // since c++14
};

using true_type	= integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

// https://en.cppreference.com/w/cpp/types/is_const
template<class T> struct is_const          : false_type {};
template<class T> struct is_const<const T> : true_type {};

// https://en.cppreference.com/w/cpp/types/is_reference

template <class T> struct is_reference      : false_type {};
template <class T> struct is_reference<T&>  : true_type {};
template <class T> struct is_reference<T&&> : true_type {};

// https://en.cppreference.com/w/cpp/types/is_function
template<class T>
struct is_function : integral_constant<
    bool,
    !is_const<const T>::value && ! is_reference<T>::value
> {};

    // https://en.cppreference.com/w/cpp/types/remove_cv
    template< class T > struct remove_cv                   { typedef T type; };
template< class T > struct remove_cv<const T>          { typedef T type; };
template< class T > struct remove_cv<volatile T>       { typedef T type; };
template< class T > struct remove_cv<const volatile T> { typedef T type; };
 
template< class T > struct remove_const                { typedef T type; };
template< class T > struct remove_const<const T>       { typedef T type; };
 
template< class T > struct remove_volatile             { typedef T type; };
template< class T > struct remove_volatile<volatile T> { typedef T type; };

// https://en.cppreference.com/w/cpp/types/remove_extent

template<class T>
struct remove_extent { typedef T type; };
 
template<class T>
struct remove_extent<T[]> { typedef T type; };
 
template<class T, size_t N>
struct remove_extent<T[N]> { typedef T type; };

// https://en.cppreference.com/w/cpp/types/remove_reference
    template< class T > struct remove_reference      {typedef T type;};
template< class T > struct remove_reference<T&>  {typedef T type;};
template< class T > struct remove_reference<T&&> {typedef T type;};

// https://en.cppreference.com/w/cpp/types/conditional
template<bool B, class T, class F>
struct conditional { typedef T type; };
 
template<class T, class F>
struct conditional<false, T, F> { typedef F type; };

// https://en.cppreference.com/w/cpp/types/add_pointer
namespace detail {
 
template <class T>
struct type_identity { using type = T; }; // or use type_identity (since C++20)
 
template <class T>
auto try_add_pointer(int) -> type_identity<typename remove_reference<T>::type*>;
template <class T>
auto try_add_pointer(...) -> type_identity<T>;
 
} // namespace detail
 
template <class T>
struct add_pointer : decltype(detail::try_add_pointer<T>(0)) {};

// https://en.cppreference.com/w/cpp/types/is_array
template<class T>
struct is_array : false_type {};
 
template<class T>
struct is_array<T[]> : true_type {};
 
template<class T, size_t N>
struct is_array<T[N]> : true_type {};

// https://en.cppreference.com/w/cpp/types/decay
    template< class T >
struct decay {
private:
    typedef typename remove_reference<T>::type U;
public:
    typedef typename conditional< 
        is_array<U>::value,
        typename remove_extent<U>::type*,
        typename conditional< 
            is_function<U>::value,
            typename add_pointer<U>::type,
            typename remove_cv<U>::type
        >::type
    >::type type;
};
} // namespace nonstd 
} // dbj