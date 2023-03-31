@echo off

cl.exe 2>nul >nul

IF %ERRORLEVEL% EQU 9009 (call setup.bat)

mkdir bin
mkdir bin\debug_obj

ctime -begin compile.ctm

cl /c /wd4312 /diagnostics:column /EHsc /std:c++20 /Fobin/debug_obj/ /Fdbin/debug_obj/ /FeRedFoxEngine_Debug.exe /FC /diagnostics:column /nologo /Zi /Od /MP /FC src\Camera.cpp src\main.cpp src\ObjParser.cpp src\OpenGLGraphics.cpp src\win32Platform.cpp src\Engine.cpp external/imgui.cpp external/imgui_draw.cpp external/imgui_widgets.cpp external/imgui_tables.cpp external/imgui_impl_win32.cpp external/imgui_impl_opengl3.cpp src/GameObject.cpp src/UI.cpp -I external/include /Iinclude/RedFoxMaths /Iexternal/include/Jolt /I include /link user32.lib opengl32.lib gdi32.lib
cl /wd4312 /diagnostics:column /EHsc /std:c++20 /Fobin/debug_obj/ /Fdbin/debug_obj/ /FeRedFoxEngine_Debug.exe /FC /diagnostics:column /nologo /Zi /Od /MP /FC external\ImGuizmo.cpp src\Camera.cpp src\main.cpp bin\debug_obj\ObjParser.obj src\OpenGLGraphics.cpp src\win32Platform.cpp src\Engine.cpp bin\debug_obj\imgui.obj bin\debug_obj\imgui_draw.obj bin\debug_obj\imgui_widgets.obj bin\debug_obj\imgui_tables.obj bin\debug_obj\imgui_impl_win32.obj bin\debug_obj\imgui_impl_opengl3.obj src/GameObject.cpp src/UI.cpp -I external/include /Iinclude/RedFoxMaths /Iexternal/include/Jolt /I include /link user32.lib opengl32.lib gdi32.lib

cl /diagnostics:column /std:c++20 /Od /LD src/Game.cpp /I external/include /I include /Iinclude/RedFoxMaths /link /OUT:game.dll

ctime -end compile.ctm %ERRORLEVEL%
