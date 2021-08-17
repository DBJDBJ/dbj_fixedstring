# DBJ FIXED_STRING

## Caveat Emptor ("Let the Buyer beware")

The purpose of this lib is Proof Of Concept aka POC. This is not "industrial strength" , "drop in replacement" (to what?) It is simply an POC.

### Fixed string with string_view front end.

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
    fixie[0] = '0';
    fixie.at(1) = '1';
    fixie.data()[2] = '2';
    return 42;
}
```

- Idea was to re-use the std::string_view  interface and implement it with added ability to muatate the char array referenced
    - thus there is no new interface to learn and get accustomed to
      - and make bugs while doing it
    - all you can do with `std::string_view` you can do with `dbj::fixed_string`
      - but not much more
- Autoslab is delivered by [dbj_collector](dbj_collector.h)
  - char block of required size, is allocated on heap and auto-freed at application exit.  
    - to achieve that, clang/gcc destructor attribute is used `__attribute__((destructor))`
    - `dbj_collector` is used through one single call
```cpp
// freed on app exit
return type((char *)dbj_collector_alloc(size_));
```
  - dbj_collector uses [utlist](https://github.com/troydhanson/uthash/blob/master/src/utlist.h) part of [troydhanson/uthash](https://github.com/troydhanson/uthash) suite
- string_view implementation is [martinmoene/string-view-lite](https://github.com/martinmoene/string-view-lite)
  - the whole suite can be used from C++11 onwards
    - if there are claims to the contrary please report in issues
  - customized by `string.view.tweak,h`

## NOTES

- This is not tested. Yet. 
- We value benchmarking over testing. 
  - Benchmarking means it has to work **and** it has to be fast.
  - We use the excellent [UBUT](https://github.com/dbj-data/ubut).
    - An combination of [ubench / utest](https://github.com/sheredom) 
- Of course all character types will be catered for. 
  - In case anybody is using anything else but `char`.

## Orthodox C++ and unorthodox C

- We do not use C++ exceptions, RTTI and iostream. Any other std:: usage is kept at minimum, at best.
- Anything that can be implemented in modern C will be implemented in modern C. 
   - With all the clang/gnuc extensions available too.
- Compilers we use are clang-cl and TDM-GCC-64. 
   - That is revealing us as developing on Windows 10, only.
- For linux compatibility proof, we (sometimes) offer Godbolt run programs.

> &copy; 2021 by dbj@dbj.org 

[LICENSE DBJ](https://dbj.org/license_dbj/)
