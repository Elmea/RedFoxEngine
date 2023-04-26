#include "GameObject.hpp"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

namespace RedFoxEngine
{
    RedFoxMaths::Mat4 GameObject::GetLocalMatrix()
    {
        return (RedFoxMaths::Mat4::CreateTransformMatrix(position, orientation, scale));
    }
}
