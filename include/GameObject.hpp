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
    enum ColliderType
    {
        CT_NONE,
        CT_SPHERE,
        CT_CUBE,
        CT_CAPSULE,
    };
    enum StateType
    {
        ST_NONE,
        ST_DYNAMIC,
        ST_STATIC,
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

        union
        {
            struct
            {
                ColliderType type;
                StateType state;
            };
            physx::PxRigidActor* body;
        };

        int behaviourIndex;
        RedFoxMaths::Mat4 GetLocalMatrix();
        void SetTransform(Transform transform);
        void UpdateTransform();
    };
}
