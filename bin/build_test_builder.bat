@echo off

SET MyPath=%~dp0
SET BinPath=%MyPath:~0,-1%
SET ProjectPath=%BinPath%\..\

set TESTS_RUN_TREE=%ProjectPath%\tests_run_tree
IF NOT EXIST %TESTS_RUN_TREE% mkdir %TESTS_RUN_TREE%
pushd %TESTS_RUN_TREE%

call ..\bin\cl_setup.bat

set CompilerFlags=-O0 -g -Wno-writable-strings

clang %CompilerFlags% -c ..\test_builder_src\first.c -o test_builder.o
link -debug .\test_builder.o user32.lib gdi32.lib msvcrt.lib msvcprt.lib opengl32.lib

popd