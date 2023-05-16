#pragma once
#include "MyMemory.hpp"
#include "ResourceManager.hpp"

namespace RedFoxEngine
{
	class Scene;
	class GameObject;
	class Input;

	#define BEHAVIOUR(name) \
	void name(RedFoxEngine::GameObject* self, const float deltaTime, RedFoxEngine::Scene *scene, RedFoxEngine::Input *input)

	typedef BEHAVIOUR(functionBehaviour);

	struct Behaviour
	{
		MyString name;
		functionBehaviour* function;
	};
}
