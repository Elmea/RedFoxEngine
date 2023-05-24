#pragma once
#include "MyMemory.hpp"
#include "ResourceManager.hpp"

namespace RedFoxEngine
{
	class Scene;
	class Physx;
	class GameObject;
	class Input;
	class WindowDimension;

	#define BEHAVIOUR(name) \
	void name(RedFoxEngine::GameObject* self, const float deltaTime, \
		RedFoxEngine::Scene *scene, \
		RedFoxEngine::Input *inputs, \
		RedFoxEngine::Physx *physx, \
		RedFoxEngine::WindowDimension *windowDimension)
	
	#define UIBEHAVIOUR(name) \
	void name(RedFoxEngine::GameUI* self, const float deltaTime, RedFoxEngine::Scene *scene, RedFoxEngine::Input *inputs)

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
