#include "GameObject.hpp"
#include "Physics.hpp"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

using namespace physx;

namespace RedFoxEngine
{
    RedFoxMaths::Mat4 GameObject::GetLocalMatrix()
    {
        return (RedFoxMaths::Mat4::CreateTransformMatrix(position, orientation, scale));
    }

	void GameObject::SetTransform(Transform transform)
	{
		PxTransform t;
		t.p = { transform.position.x, transform.position.y, transform.position.z };
		t.q = { transform.orientation.b, transform.orientation.c, transform.orientation.d, transform.orientation.a };
		body->setGlobalPose(t);
		PxShape* bodyShape;
		body->getShapes(&bodyShape, 1);
		if (&bodyShape->getGeometry())
		{
			switch (bodyShape->getGeometryType())
			{
			case PxGeometryType::eBOX:
				bodyShape->setGeometry(PxBoxGeometry(transform.scale.x, transform.scale.y, transform.scale.z));
				break;

			case PxGeometryType::eSPHERE:
				bodyShape->setGeometry(PxSphereGeometry(transform.scale.x));
				break;
			}
		}
	}

    void GameObject::UpdateTransform()
    {
        physx::PxTransform bodyTransform = body->getGlobalPose();
        transform.position = { bodyTransform.p.x, bodyTransform.p.y, bodyTransform.p.z };
        transform.orientation = { bodyTransform.q.w, bodyTransform.q.x, bodyTransform.q.y, bodyTransform.q.z };
    }
}
