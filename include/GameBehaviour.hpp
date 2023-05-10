#pragma once
#include "MyMemory.hpp"
#include "ResourceManager.hpp"

namespace RedFoxEngine
{

class Scene;

#define BUTTONBEHAIVOUR(name) \
void name(RedFoxEngine::Scene *scene)

typedef BUTTONBEHAIVOUR(functionBehaviour);

class GameBehaviour
{

public:
	MyString name;
	functionBehaviour *function;
};

}
