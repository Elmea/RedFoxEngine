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
        physx::PxTransform bodyTransform = body->getGlobalPose();
        transform.position = { bodyTransform.p.x, bodyTransform.p.y, bodyTransform.p.z };
        transform.orientation = { bodyTransform.q.w, bodyTransform.q.x, bodyTransform.q.y, bodyTransform.q.z };
    }
}
