@echo off

cl.exe 2>nul >nul

IF %ERRORLEVEL% EQU 9009 (call setup.bat)

mkdir bin
mkdir bin\release_obj
mkdir bin\debug_obj

ctime -begin compile.ctm

cl /Fobin/release_obj/ /Fdbin/release_obj/ /FeRedFoxEngine.exe /std:c++14 /FC /diagnostics:column /nologo /Zi /O2 /MP /FC src\main.cpp src\ObjParser.cpp src\OpenGLGraphics.cpp src\win32Platform.cpp src\Engine.cpp /I external/include /I include /link user32.lib opengl32.lib gdi32.lib

rem cl /std:c++14 /FC /diagnostics:column /nologo /Zi /Od /MP /FC src\main.cpp src\ObjParser.cpp src\OpenGLGraphics.cpp src\win32Platform.cpp src\Engine.cpp /I external/include /I include /link user32.lib opengl32.lib gdi32.lib

ctime -end compile.ctm %ERRORLEVEL%