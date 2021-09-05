# DBJ FIXED_STRING<!-- omit in toc -->

- [Fixed string with string_view front end.](#fixed-string-with-string_view-front-end)
  - [Errors handling policy](#errors-handling-policy)
  - [But,but, what about ... ?](#butbut-what-about--)
  - [NOTES](#notes)
- [Orthodox C++ and unorthodox C](#orthodox-c-and-unorthodox-c)
- [Dependencies](#dependencies)
- [Building](#building)
  - [Notes on building](#notes-on-building)
- [Debugging](#debugging)
  
# &nbsp;<!-- omit in toc -->

> Caveat Emptor ("Let the Buyer beware")

The purpose of this lib is Proof Of Concept aka POC. This is not "industrial strength" , "drop in replacement" (to what?) It is simply an POC.

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
    // with the added ability to mutate
    // through only these 3 methods
    fixie[0] = '0';
    fixie.at(1) = '1';
    fixie.data()[2] = '2';
    return 42;
}
```

- Idea was to re-use the std::string_view  interface and implement it with added ability to mutate the char array referenced aka Autoslab.
    - thus there is no new interface to learn and get accustomed to
      - and make bugs while doing it
    - all you can do with `std::string_view` you can do with `dbj::fixed_string`
      - but not much more
- Autoslab is delivered by [dbj_collector](dbj_collector.h)
  - char block of required size, is allocated on heap and auto-freed at application exit.  
    - to achieve that, clang/gcc destructor attribute is used `__attribute__((destructor))`
    - `dbj_collector` is used through one single call
```cpp
// allocate the autoslab of required size; freed on app exit
return type((char *)dbj_collector_alloc(size_));
```
  - dbj_collector uses [utlist](https://github.com/troydhanson/uthash/blob/master/src/utlist.h) part of [troydhanson/uthash](https://github.com/troydhanson/uthash) suite
- string_view implementation is [martinmoene/string-view-lite](https://github.com/martinmoene/string-view-lite)
  - the whole suite can be used from C++11 onwards
    - if there are claims to the contrary please report in issues
  - customized by `string.view.tweak,h`

## Errors handling policy

- Error handling is a folly
  - the only effective approach is returns handling
  - on both sides: returning and receiving.
- safe and fast code does not exist (and probably never will)
  - right now if you do silly things with `dbj::fixed_string` bad things will happen in return
- in the future we/I will provide a separate implementation that wil use [dbj::valstat](https://valstat.github.io/home/)
  - that will be safe and not-that-fast

## But,but, what about ... ?

- Having "better" front end? Notably the one of `std::string` ? 
  - this is **fixed** string we are developing here
  - you know you can not just append to it, or adding them fixed string to each other. Right?
  - on the other hand, assigning to the fixed_string in general has value to **some** users
  - just please go ahead and implement that simple function for them users who want it
  ```cpp
  // you are more than capable to implement this
  /*auto fixie =*/ assign( fixie, "DATA" ) ;
  ```
  - Developing std::string API clone would be one much more involved fixed_string library for no obvious gains
- Using `std::array` or `std::` whatever to implement?
  - that increases the exposure to the `std::` lib
    - in our/my books "not a good thing"

## NOTES

- We use the excellent [UBUT](https://github.com/dbj-data/ubut).
  - An (almost refactored) combination of [ubench/utest](https://github.com/sheredom) 
  - Beware: it is developed by me
      - It is Windows only
      - It is not guaranteed to work with GCC
      - It is in pre-beta stage
- Of course all character types will be catered for 
  - In case anybody is using anything else but `char`
- There is a memory safety product called [AUTOSLAB](https://grsecurity.net/how_autoslab_changes_the_memory_unsafety_game), which I did not know about before today.
  - I use the name here as a catchy phrase for auto-freeing-slab of memory

# Orthodox C++ and unorthodox C

- We/I do not use C++ exceptions, RTTI and iostream. 
   - Any other `std::` usage is kept at minimum, at best.
- Anything that can be implemented in modern C will be implemented in modern C. 
   - With all the `clang`/`gnuc` extensions available too.
- Compilers we/I use are `clang-cl` and `TDM-GCC-64`. 
   - That is revealing us/me as developing on Windows 10, only.
- For linux compatibility proof, we/I (sometimes) offer Godbolt demos.

# Dependencies

- https://github.com/dbj-data/machine_wide
    - check it out somewhere on your machine
    - add it to the `INCLUDE` env var
    - add ti to the  `IncludePath` in `.vscode/c_cpp_properties.json`

# Building

- `clang-cl` is mandatory
- C++17 is mandatory
- use `build_fixed_string_poc.cmd`
- output goes to the `bin` folder

## Notes on building

VS Code C/C++ compiling using nothing but `.vscode/` stuff is one very deep rabbit hole. For building inside VS Code, use batch scripts and for complex projects use Visual Studio.

What they do not tell you in the school about CMake is: it is good but ONLY for multi platform projects. We have seen many beginners jumping head first into CMake without first realising what is it actually for. Another reason people do use CMake is: there is nothing like Visual Studio on Linux. There is CLion but that is just one large front end to CMake; written in Java. And it is written in Java by the inventors of Kotlin. Why?

There are many Linux Devops teams using VisualStudio and Windows10 workstations just because of Visual Studio. Please [proceed here](https://devblogs.microsoft.com/cppblog/linux-development-with-c-in-visual-studio-2019-wsl-asan-for-linux-separation-of-build-and-debug/).

# Debugging
- position yourself on the `fixed_string_poc.cpp`
- goto debugger and start debugging

> &copy; 2021 by dbj@dbj.org 
[LICENSE_DBJ](https://dbj.org/license_dbj/) 
