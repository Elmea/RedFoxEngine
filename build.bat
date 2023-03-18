@echo off

cl.exe 2>nul >nul

IF %ERRORLEVEL% EQU 9009 (call setup.bat)

mkdir bin
mkdir bin\release_obj
mkdir bin\debug_obj

ctime -begin compile.ctm

rem cl /diagnostics:column /Fobin/release_obj/ /Fdbin/release_obj/ /FeRedFoxEngine.exe /std:c++14 /FC /diagnostics:column /nologo /Zi /O2 /MP /FC src\main.cpp src\ObjParser.cpp src\OpenGLGraphics.cpp src\win32Platform.cpp src\Engine.cpp external/imgui.cpp external/imgui_draw.cpp external/imgui_widgets.cpp external/imgui_tables.cpp /I external/include /I include /link user32.lib opengl32.lib gdi32.lib

cl /diagnostics:column /Fobin/debug_obj/ /Fdbin/debug_obj/ /FeRedFoxEngine_Debug.exe /std:c++14 /FC /diagnostics:column /nologo /Zi /Od /MP /FC src\Camera.cpp src\main.cpp src\ObjParser.cpp src\OpenGLGraphics.cpp src\win32Platform.cpp src\Engine.cpp external/imgui.cpp external/imgui_draw.cpp external/imgui_widgets.cpp external/imgui_tables.cpp external/imgui_impl_win32.cpp external/imgui_impl_opengl3.cpp src/GameObject.cpp src/UI.cpp -I external/include /I include /link user32.lib opengl32.lib gdi32.lib

cl /Od /LD src/Game.cpp /I external/include /I include /link /OUT:game.dll

ctime -end compile.ctm %ERRORLEVEL%
