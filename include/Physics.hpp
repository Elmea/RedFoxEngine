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
        int actorCount = 0;
    private:
    public:
        void CreateCubeCollider(GameObject* object, physx::PxU32 size, physx::PxReal halfExtent);
        void CreateSphereCollider(GameObject* object);
        void CreateCapsuleCollider(GameObject* object, physx::PxF32 radius, physx::PxF32 halfHeight);
        void InitPhysics(Scene scene, int sphereIndex);
        virtual void SetTransform(int index, Transform transform);
        void UpdatePhysics(f32 deltaTime, Scene* scene, ResourcesManager m);

        ~Physx()
        {
            if (cudaContextManager)
                cudaContextManager->release();
        };
    };

}
