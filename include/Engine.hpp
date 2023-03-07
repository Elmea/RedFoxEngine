/* date = February 22nd 2023 2:44 pm */

#pragma once

#ifdef _WIN32
#include "Win32Platform.hpp"
#else
// TODO(V. Caraulan): linux mac or whatever
#endif

#include "OpenGLGraphics.hpp"
#include "ObjParser.hpp"

namespace RedFoxEngine
{

	class Engine
	{
		private:
		u64 		m_startingTime;
		Memory	m_arenaAllocator;
		Model		*m_models;
		u64			m_modelCount;
		Input		m_input;
		u64			m_frequency;
		public:
		Platform	m_platform; // TODO(V. Caraulan): make private
		Graphics	m_graphics;
		u64				m_time;
		f64				m_deltaTime;
		private:
		public:
		Engine(int width, int height);
		~Engine();
		void GetInputs();
		void Update();
		void Draw();
		void ObjModelPush(const char *objPath);
		void SetViewProjectionMatrix(RedFoxMaths::Mat4 viewProjection);
	};
}
