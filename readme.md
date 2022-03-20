# DBJ FIXED_STRING<!-- omit in toc -->

- [Fixed string with string_view front end.](#fixed-string-with-string_view-front-end)
    - [Perhaps some reasoning?](#perhaps-some-reasoning)
  - [Errors handling policy](#errors-handling-policy)
  - [But,but,but ... what about ... ?](#butbutbut--what-about--)
  - [NOTES](#notes)
- [Orthodox C++ and unorthodox C](#orthodox-c-and-unorthodox-c)
- [Building](#building)
  - [Notes on building](#notes-on-building)
    - [But why not CMake?](#but-why-not-cmake)
- [Debugging](#debugging)
  
# &nbsp;<!-- omit in toc -->

> Caveat Emptor ("Let the Buyer beware")

The purpose of this lib is Proof Of Concept aka POC. This is not "industrial strength" , "drop in replacement" (to what?) It is simply an POC.

Maybe even the title is wrong or even "misleading"?

# Fixed string with string_view front end.

```cpp
#include <fixed_string.h>

int main (void)
{
    // create and use the "autoslab"
    // char mem block heap allocated and referenced
    // and auto freed at app exit
    auto fixie = dbj::fixed_string::make(0xFF) ;
    // exactly the same interface as std::string_view
    // with the added minimal ability to mutate
    // through only these 3 methods
    fixie[0] = '0';
    fixie.at(1) = '1';
    fixie.data()[2] = '2';
    
    // prints: 012
    std::cout <<  fixie.data() ;

    return 42;
}
```

### Perhaps some reasoning?

- Idea was to re-use the std::string_view  interface and implement it with added ability to mutate the char array referenced aka Autoslab.
    - thus there is no new interface to learn and get accustomed to
      - and make bugs while doing it
    - all you can do with `std::string_view` you can do with `dbj::fixed_string`
      - and not much more
    - beside living in fear that `char[]` used will be somehow lost
      - unless you deliberately lose it
- Autoslab is delivered by [dbj_collector](dbj_collector.h)
  - for each allocation, char block of required size, is allocated on heap and auto-freed at application exit.  
    - to achieve that, clang/gcc destructor attribute is used `__attribute__((destructor))`
    - `dbj_collector` , through one single destructor function
```cpp
// allocate the autoslab of required size; auto-freed on app exit
return type((char *)dbj_collector_alloc(size_));
```
  - dbj_collector uses [uthash](https://github.com/troydhanson/uthash/blob/master/src/uthash.h) part of [troydhanson/uthash](https://github.com/troydhanson/uthash) suite
- string_view implementation is [martinmoene/string-view-lite](https://github.com/martinmoene/string-view-lite)
  - the whole suite can be used from C++11 onwards
    - if there are claims to the contrary please report in issues
  - customized by `string.view.tweak,h`

## Errors handling policy

- Error handling is a folly
  - the only effective approach is returns handling
  - on both sides: returning and receiving side of the function call.
- safe and fast code does not exist (and probably never will)
  - right now if you do silly things with `dbj::fixed_string` bad things will happen in return
- in the future we/I will provide a separate implementation that wil use [dbj::valstat](https://valstat.github.io/home/)
  - that will be safe and not-that-fast

## But,but,but ... what about ... ?

- Having "better" front end? Notably the one of `std::string` ? 
  - this is **fixed** string we are developing here
  - you know you can not just append to it, or adding them fixed string to each other. Right?
  - on the other hand, assigning to the fixed_string in general has value to **some** users
  - just please go ahead and implement that simple function for them users who want it from you:
  ```cpp
  // you are more than capable to implement this 
  // by using dbj::fixed_string
  // in more that one way
  
  /*auto fixstr =*/ assign( fixie, "DATA" ) ;
  ```
- Developing std::string API clone would be one much more involved fixed_string library for no obvious gains
- But, why not using `std::array` or `std::` whatever to implement this?
  - that increases the exposure to the `std::` lib
  - in our/my books that is "not a good thing"
  - by now (2021Q4) `std::` looks like an accidental pile-up, unfortunately

## NOTES

- We use the excellent [UBUT](https://github.com/dbj-data/ubut).
  - An (almost completely refactored) combination of [ubench/utest](https://github.com/sheredom) 
  - Beware: it is developed by me :wink:
      - It is Windows only
      - It is not guaranteed to work with GCC
      - It is in pre-beta stage
- Of course all character types will be catered for 
  - In case anybody is using anything else but `char`

Nota Bene: There is a memory safety product called [AUTOSLAB](https://grsecurity.net/how_autoslab_changes_the_memory_unsafety_game), which I did not know about before today.
  - I use the name here as a catchy phrase for auto-freeing, slab of memory

# Orthodox C++ and unorthodox C

Let us be honest with you :wink:

- We do not use C++ exceptions, RTTI and iostream. 
   - Any other `std::` usage is kept at minimum, at best. 
   - Zero `std::` is the aim.
- Saying all of that, we have to admit "zero C++" is the actual aim
- In here, anything that can be implemented in modern C will be implemented in modern C. 
   - With all the `clang`/`gnuc` extensions available too.
- Compilers we predominantly use are `clang-cl` and `TDM-GCC-64`. 
   - That is rendering us as developing on Windows 10, only.
- For linux compatibility proof, we (sometimes) offer Godbolt demos.

Very good UTHASH library is used. Although extensions had to be added for it to be able to deal with `unsigned char []`. See the `dbj_uthash.h`.

Also: https://godbolt.org/z/fKxfYqv5r

# Building

- `clang-cl` is mandatory
- C++17 is probably mandatory
- use `build_fixed_string_poc.cmd`
- output goes to the `bin` folder

## Notes on building

VS Code C/C++ compiling using nothing but `.vscode/` stuff is one very deep rabbit hole. For building inside VS Code, use batch scripts and for complex projects use Visual Studio.
Although we divide complex projects on DLL's which in turn we build in a very simple way.

### But why not CMake?

What they do not tell you in the school about CMake is: it is good but ONLY for multi platform projects. We have seen many beginners jumping head first into CMake without first realising what is it actually for.  Here is the truth:

> On Windows CMake creates Visual Studio solutions; it does not build executables

Another reason people do use CMake is: there is nothing like Visual Studio on Linux. There is CLion but that is just one large front end to CMake; written in Java. (And it is written in Java by the inventors of Kotlin. Why?)

There are many Linux Devops teams using VisualStudio and Windows10 workstations just because of Visual Studio. Please [proceed here](https://devblogs.microsoft.com/cppblog/linux-development-with-c-in-visual-studio-2019-wsl-asan-for-linux-separation-of-build-and-debug/).

# Debugging
- position yourself on the `fixed_string_poc.cpp`
- goto debugger and start debugging

> &copy; 2021 by dbj@dbj.org 
[LICENSE_DBJ](https://dbj.org/license_dbj/) 
