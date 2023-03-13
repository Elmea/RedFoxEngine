#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"
#include "Engine.hpp"

int main()
{
    RedFoxEngine::Engine engine(1920, 1080);

    while (engine.isRunning())
    {
        engine.ProcessInputs();
        engine.Update();
        engine.Draw();
    }
    return (0);
}
