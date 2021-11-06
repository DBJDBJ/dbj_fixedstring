
@cls
@setlocal

@if [%1] == [clean] goto clean

@rem Setup VS2019
@rem change to your local path, if need be
call "D:\PROD\programs\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

@REM @set "INCLUDE=D:\machine_wide;%INCLUDE%"

@set "basename=dbj_collector_test"

@if [%1] == [release] goto build_release

@rem default is goto build_debug

:: default
:build_debug
clang-cl /std:c17 /GR- /J /MTd /Zi -D_DEBUG %basename%.c /o ../out/%basename%.exe 
@goto exit

:build_release
clang-cl /std:c17 /GR- /J /O2 /MT -DNDEBUG %basename%.cpp /o ../out/%basename%.exe
@goto exit

:clean
@del ..\out\*.pdb
@del ..\out\*.ilk
@del ..\out\*.exe
@goto exit

@endlocal
:exit
