@echo off

pushd C:\projects\gs_libs\tests_run_tree
call ..\bin\cl_setup.bat

set CompilerFlags=-O0 -g -fdiagnostics-absolute-paths
set LinkerLibraries=user32.lib gdi32.lib msvcrt.lib msvcprt.lib opengl32.lib

set TestFileNoExtensions=compiles_cpp.test
set TestFileFullPath=..\tests_src\compiles_cpp.test.cpp
clang %CompilerFlags% -c %TestFileFullPath%
link /NOLOGO -debug %TestFileNoExtensions%.o %LinkerLibraries%
%TestFileNoExtensions%.exe
echo.

popd