#pragma once
#include "ObjParser.hpp"
#include "RedfoxMaths.hpp"
#include "Model.hpp"
#include "Transform.hpp"

namespace physx
{
    class PxRigidActor;
}

namespace RedFoxEngine
{
    struct GameObject
    {
        MyString name;
        union
        {
            Transform transform;
            struct
            {
              RedFoxMaths::Float3 position;
              RedFoxMaths::Float3 scale;
              RedFoxMaths::Quaternion orientation;
            };
        };
        int parent;
        int modelIndex;
        RedFoxMaths::Float3 Color;
        
        physx::PxRigidActor* body;

        int behaviourIndex;

        RedFoxMaths::Mat4 GetLocalMatrix();
        void SetTransform(Transform transform);
        void UpdateTransform();
    };
}
