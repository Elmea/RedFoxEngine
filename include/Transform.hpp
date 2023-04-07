#pragma once
#include "RedfoxMaths.hpp"
#include "Model.hpp"

#include <PhysX/PxConfig.h>
#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/PxRigidBody.h>

using namespace physx;
using namespace RedFoxEngine;

namespace RedFoxEngine
{
    struct Transform
    {        
        RedFoxMaths::Float3 position;
        RedFoxMaths::Float3 scale;
        RedFoxMaths::Quaternion orientation;



        Transform operator=(const Transform& pOther);
        Transform operator=(const PxTransform& pOther);
		
		void assign(PxTransform other);

    };
}

