@echo off

SET LocalEmsdkPath=C:\apps\emsdk
IF NOT DEFINED EMSDK (call %LocalEmsdkPath%\emsdk_env.bat)