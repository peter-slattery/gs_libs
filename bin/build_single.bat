@echo off

pushd C:\projects\gs_libs\tests_run_tree
call ..\bin\cl_setup.bat
call .\build_single_test.bat
popd