@echo off

IF NOT EXIST C:\projects\gs_libs\tests_run_tree mkdir C:\projects\gs_libs\tests_run_tree

set CompilerFlags=-O0 -g -DDEBUG=1
set LinkerLibraries=user32.lib gdi32.lib msvcrt.lib msvcprt.lib opengl32.lib

pushd C:\projects\gs_libs\tests_run_tree 

call ..\bin\cl_setup.bat

clang %CompilerFlags% -c C:\projects\gs_libs\src\meta\gs_meta.c
link /NOLOGO -debug gs_meta.o %LinkerLibraries%

popd