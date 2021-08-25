@echo off

SET MyPath=%~dp0
SET BinPath=%MyPath:~0,-1%
SET ProjectPath=%BinPath%\..\

pushd %ProjectPath%\tests_run_tree
call ..\bin\cl_setup.bat

set CompilerFlags=-O0 -g -fdiagnostics-absolute-paths
set LinkerLibraries=user32.lib gdi32.lib msvcrt.lib msvcprt.lib opengl32.lib

set TestFileNoExtensions=gs_hash.test
set TestFileFullPath=..\tests_src\gs_hash.test.c
clang %CompilerFlags% -c %TestFileFullPath%
link /NOLOGO -debug %TestFileNoExtensions%.o %LinkerLibraries%
%TestFileNoExtensions%.exe
echo.

popd