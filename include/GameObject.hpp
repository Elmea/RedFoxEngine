#pragma once
#include "ObjParser.hpp"
#include "RedfoxMaths.hpp"
#include "Model.hpp"

#include <PhysX/PxConfig.h>
#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/PxRigidBody.h>

using namespace physx;

namespace RedFoxEngine
{
    struct GameObject
    {
        char* name;
        GameObject* parent;
        Model *model;
        RedFoxMaths::Float3 position;
        RedFoxMaths::Float3 scale;
        RedFoxMaths::Quaternion orientation;
        
        PxRigidBody* rb;

    public:

        RedFoxMaths::Mat4 GetLocalMatrix();
        RedFoxMaths::Mat4 GetWorldMatrix();
        GameObject **GetChildren(GameObject *gameObjects, int modelCount,
            Memory *temp);
        

        int GetChildrenCount(GameObject *objects, int objectCount);
        
        void Test();
    };
}
