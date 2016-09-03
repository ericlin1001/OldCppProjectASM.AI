@echo off
set /p name=Name:
copy ..\Release\RenjuTest.exe %name%.exe
start %name%.exe