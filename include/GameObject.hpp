#pragma once
#include "ObjParser.hpp"
#include "RedfoxMaths.hpp"
#include "Model.hpp"
#include "Transform.hpp"
#include "Material.hpp"

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
        // int materialIndex;
        // int materialIndex1;
        // int materialIndex2;
        RedFoxMaths::Float3 Color;
        
        physx::PxRigidActor* body;

        int behaviourIndex;
        // RedFoxEngine::Material material;
        RedFoxMaths::Mat4 GetLocalMatrix();
        void SetTransform(Transform transform);
        void UpdateTransform();
    };
}
