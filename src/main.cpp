#include "Engine.hpp"

int main()
{
	RedFoxEngine::Engine engine(1920, 1080);

//	ParsingObject model;
//	engine.m_graphics.InitModel(&model);
	while(engine.m_platform.m_running)
	{
		engine.GetInputs();
		engine.Update();
		engine.Draw();
	}
	return(0);
}
