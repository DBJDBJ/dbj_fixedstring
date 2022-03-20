
:: VS CODE NOTE: even if clang-cl.exe is on the path and callable from the cmd line
:: you can not use it as such from VS CODE
:: you can use cl.exe and you can use g++ is you have it and it is on the path
:: but you can not do the same with clang as installed as part of V.Sudio 2019
:: ditto this batch script

:: this is for C builds
:: clang.exe
:: -xc is per file
:: -xc++ is per file
:: https://clang.llvm.org/docs/CommandGuide/clang.html
:: The default C++ language standard is gnu++14.
:: The default C language standard is gnu17
:: set MS_EXTENSIONS=-fms-extensions
::
:: -Os Like -O2 with extra optimizations to reduce code size.
:: -Oz Like -Os (and thus -O2), but reduces code size further.

:: clang-cl
:: https://clang.llvm.org/docs/UsersManual.html#clang-cl

@cls
@setlocal

@if [%1] == [clean] goto clean

@rem Setup VS2019
@rem change to your local path, if need be
call "D:\PROD\programs\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

@REM @set "INCLUDE=D:\machine_wide;%INCLUDE%"

@set "basename=fixed_string_poc"

@if [%1] == [release] goto build_release

@rem default is goto build_debug

:: default
:build_debug
clang-cl /std:c++17 /GR- /J /MTd /Zi -D_DEBUG %basename%.cpp /o out/%basename%.exe 
@goto exit

:build_release
clang-cl /std:c++17 /GR- /J /O2 /MT -DNDEBUG %basename%.cpp /o out/%basename%.exe
@goto exit

@rem IF USING ./vector add on both lines above vector/vector.c

:clean
@del *.filters
@del *.user
@del out\*.pdb
@del out\*.ilk
@del out\*.exe
@goto exit

@endlocal
:exit
