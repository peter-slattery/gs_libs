@echo off

IF NOT EXIST .\build\ mkdir .\build

set CommonCompilerFlags=-DDEBUG=1 -nologo -FC -WX -W4 -Z7 -Oi -GR- -EHsc -EHa- -wd4127 -wd4702 -wd4101 -wd4505 -wd4100 -wd4189 -wd4244 -wd4201 -wd4996 -I%CommonLibs% -dDEBUG -Od
set CommonLinkerFlags= -opt:ref 

pushd build
cl %CommonCompilerFlags% ..\src\first.cpp  /link %CommonLinkerFlags%
popd