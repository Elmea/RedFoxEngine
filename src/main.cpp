#include "Engine.hpp"

int main()
{
    RedFoxEngine::Engine engine(1280, 720);

    while (engine.isRunning())
    {
        engine.Update();
        engine.Draw();
    }

    return (0);
}