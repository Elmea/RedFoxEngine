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
	
	#define UIBEHAVIOUR(name) \
	void name(RedFoxEngine::GameUI* self, const float deltaTime, RedFoxEngine::Scene *scene, RedFoxEngine::Input *input)

	typedef BEHAVIOUR(functionBehaviour);
	typedef UIBEHAVIOUR(uiBehaviour);

	struct Behaviour
	{
		MyString name;
		functionBehaviour* function;
	};

	struct UIBehaviour
	{
		MyString name;
		uiBehaviour* function;
	};
}
