@echo off

set TESTS_RUN_TREE=C:\projects\gs_libs\tests_run_tree\
IF NOT EXIST %TESTS_RUN_TREE% mkdir %TESTS_RUN_TREE%

pushd %TESTS_RUN_TREE%
test_builder.exe C:\projects\gs_libs\tests_src\
call ..\bin\cl_setup.bat
call build_and_run_tests.bat
popd
