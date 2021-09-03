@rem
@rem
@echo off
@cls
setlocal
set "sourcebasename=fixed_string_poc"
set "build=.\bin"
@md %build% 2>nul

@set "INCLUDE=F:\machine_wide;%INCLUDE%"

@rem /Zi -- debug builds + pdb file
@rem /TC -- threat all as C
@clang-cl vector\vector.c %sourcebasename%.cpp /std:c++17 /I "F:\machine_wide" /GR- /MDd /D_DEBUG /D_HAS_EXCEPTIONS=0 /nologo /Zi /W3 /Fo%build%\ /Fe%build%\%sourcebasename%.exe 

@echo.
@echo Done building: %build%\%sourcebasename%.exe
@echo.

endlocal