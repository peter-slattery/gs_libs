@echo off

SET MyPath=%~dp0
SET BinPath=%MyPath:~0,-1%
SET ProjectPath=%BinPath%\..\

set TESTS_RUN_TREE=%ProjectPath%\tests_run_tree\
IF NOT EXIST %TESTS_RUN_TREE% mkdir %TESTS_RUN_TREE%

pushd %TESTS_RUN_TREE%
test_builder.exe %ProjectPath%\tests_src\
call ..\bin\cl_setup.bat
call build_and_run_tests.bat
popd
