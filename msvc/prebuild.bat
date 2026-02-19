@echo off
rem %1 = mpn directory (generic, x86\... or x86_64\... or arm64\...)
rem %2 = platform (win32, x64/amd64, or arm64)
rem %3 = MSVC version number (e.g. 14)

set "platform="
set "bdir="

if /i "%2"=="win32" (
    set "platform=win32"
    set "bdir=x86w\"
) else if /i "%2"=="x64" (
    set "platform=x64"
    set "bdir=x86_64w\"
) else if /i "%2"=="arm64" (
    set "platform=arm64"
    set "bdir=arm64w\"
) else (
    call :seterr & echo ERROR: Unsupported platform "%2" (expected win32, x64/amd64, or arm64) & exit /b %errorlevel%
)

set "sdir="
if /i "%1"=="gc" (
    rem Generic C (portable) implementation
    set "sdir=generic"
    set "bdir=generic"
) else (
    set "sdir=%bdir%%~1"
)

if not exist "..\mpn\%sdir%" (
    call :seterr & echo ERROR: %1 is not supported & exit /b %errorlevel%
)

echo building MPIR for %1 (%platform%) from directory mpn\%sdir%

set "cdir=vs%3\cdata\mpn\%sdir%\"
set "sdir=..\mpn\%sdir%\"
set "bdir=..\mpn\%bdir%\"

call gen_mpir_h %platform%
call gen_config_h %cdir%

if exist "%sdir%\gmp-mparam.h" (
    call out_copy_rename "%sdir%\gmp-mparam.h" "..\" "gmp-mparam.h"
) else (
    call out_copy_rename "%bdir%\gmp-mparam.h" "..\" "gmp-mparam.h"
)

type "..\longlong_pre.h" > tmp.h
type "%bdir%\longlong_inc.h" >> tmp.h
type "..\longlong_post.h" >> tmp.h
call out_copy_rename tmp.h "..\" "longlong.h"
del tmp.h

exit /b 0

:seterr
exit /b 1
