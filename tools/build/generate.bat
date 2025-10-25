@ECHO OFF

set BUILD_DIR=%~dp0..\..\build

if not exist %BUILD_DIR% mkdir %BUILD_DIR%

pushd %BUILD_DIR%

cmake %~dp0 -G="Ninja" || goto done

:done
popd