# DBJ FIXED_STRING

```cpp
#include <fixed_string.h>

int main (void)
{
    auto fixie = dbj::fixed_string::make(0xFF) ;
    // exactly the same
    // interface as std::string_view
    // with the ability to mutate the char
    // block heap allocated and referenced
    fixie[0] = '0';
    fixie.at(1) = '1';
    fixie.data()[2] = '2';
// auto freed at app exit
    return 42;
}
```

- Idea was to re-use the std::string_view  interface and implement it so that the char array referenced can be mutated
    - thus there is no new interface to get accustomed to
    - all you can do with `std::string_view` you can do with `dbj::fixed_string`
- Delivered by [dbj_collector](dbj_collector.h), char block of required size, is allocated on heap and auto-freed at application exit.  
  - clang/gcc destructor attribute is used
```cpp
return type((char *)dbj_collector_alloc(size_));
```
- dbj_collector uses [utlist](https://github.com/troydhanson/uthash/blob/master/src/utlist.h) part of [troydhanson/uthash](https://github.com/troydhanson/uthash) suite
- string_view implementation is [martinmoene/string-view-lite](https://github.com/martinmoene/string-view-lite)
  - thus the whole suite can be used from C++11 onwards

NOTE: This is not tested. 

### Orthodox C++

We do not use exceptions, RTTI and iostream. Any other std:: usage is kept at minimum, at best.

Anything that can be implemented in modern C will be implemented in modern C.

> &copy; 2021 by dbj@dbj.org 

[LICENSE DBJ](https://dbj.org/license_dbj/)