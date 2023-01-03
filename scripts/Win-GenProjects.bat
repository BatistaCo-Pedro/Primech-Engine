@echo off 
pushd ..\
call globalVendor\premake\premake5.exe vs2022
popd
PAUSE