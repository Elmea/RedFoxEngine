@echo off
cl.exe 2>nul >nul
IF %ERRORLEVEL% EQU 9009 (call setup.bat)
cl /FC /diagnostics:column /MP /w /nologo /Zi /Od /Iexternal/include /Iinclude src/test.cpp /OUT:test.exe