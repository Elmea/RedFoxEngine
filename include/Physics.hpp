#pragma once
#include <PhysX/PxConfig.h>
#include <PhysX/PxPhysicsAPI.h>

#include "GameObject.hpp"
#include "Scene.hpp"
#include "ResourceManager.hpp"

namespace RedFoxEngine
{

    class Physx
    {
    private:
        physx::PxDefaultAllocator allocator;
        physx::PxDefaultErrorCallback errorCallback;
        physx::PxFoundation* foundation;
        physx::PxPhysics* physics;
        physx::PxDefaultCpuDispatcher* dispatcher;
        physx::PxMaterial* material;
        physx::PxPvd* pvd;
        physx::PxCudaContextManager* cudaContextManager;
    public:
        physx::PxScene* m_scene;
        physx::PxRigidActor** actors;
        physx::PxControllerManager* controllerManager;
        int actorCount = 0;
    private:
    public:
        bool gpuSimulated;
        void CreateCubeCollider(GameObject* object, physx::PxU32 size, physx::PxReal halfExtent);
        void CreateSphereCollider(GameObject* object);
        void CreatePlayerCollider(GameObject* object, physx::PxF32 radius, physx::PxF32 height, physx::PxF32 contactOffset, physx::PxF32 stepOffset, physx::PxF32 maxJumpHeight);
        void InitPhysics(Scene scene, int sphereIndex);
        virtual void SetTransform(int index, Transform transform);
        void UpdatePhysics(f32 deltaTime, ResourcesManager m, bool isPaused);

        ~Physx()
        {
            if (cudaContextManager)
                cudaContextManager->release();
        };
    };

}
