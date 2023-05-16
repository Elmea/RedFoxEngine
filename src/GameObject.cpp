#include "GameObject.hpp"
#include "Physics.hpp"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

namespace RedFoxEngine
{
    RedFoxMaths::Mat4 GameObject::GetLocalMatrix()
    {
        return (RedFoxMaths::Mat4::CreateTransformMatrix(position, orientation, scale));
    }

    void GameObject::UpdateTransform()
    {
        physx::PxTransform bodyTransform = {
            { transform.position.x, transform.position.y, transform.position.z },
            { transform.orientation.a, transform.orientation.b, transform.orientation.c, transform.orientation.d }
        };
        body->setGlobalPose(bodyTransform);
    }
}
