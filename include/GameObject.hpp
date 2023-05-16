#pragma once
#include "ObjParser.hpp"
#include "RedfoxMaths.hpp"
#include "Model.hpp"

namespace physx
{
    class PxRigidDynamic;
    class PxController;
}

namespace RedFoxEngine
{
    struct Transform
    {
        RedFoxMaths::Float3 position;
        RedFoxMaths::Float3 scale;
        RedFoxMaths::Quaternion orientation;
    };
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
        
        physx::PxRigidDynamic* body;

        int behaviourIndex;

        RedFoxMaths::Mat4 GetLocalMatrix();
        void UpdateTransform();
    };
}
