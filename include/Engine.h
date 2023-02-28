/* date = February 22nd 2023 2:44 pm */

#pragma once

#ifdef _WIN32
#include "Win32Platform.h"
#else
// TODO(V. Caraulan): linux mac or whatever
#endif

#include "OpenGLGraphics.h"
#include "ObjParser.h"
//
namespace RedFoxEngine
{

	class Engine
	{
private:
		LARGE_INTEGER m_startingTime;
		Memory		m_arenaAllocator;
		ObjModel	 *m_models;
		u64		   m_modelCount;
		Input		 m_input;
		u64		   m_frequency;
public:
		Platform m_platform; // TODO(V. Caraulan): make private
		Graphics m_graphics;
		u64	  m_time;
		f64	  m_deltaTime;
private:
public:
		Engine(int width, int height);
		~Engine();
		void GetInputs();
		void Update();
		void Draw();
		// TODO(V. Caraulan): separate opengl
		// TODO(V. Caraulan): implement functions for vertex/index buffers, texture loading ... etc
	};
}
