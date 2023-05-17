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
        physx::PxScene* m_scene = nullptr;
        int actorCount = 0;
    private:
    public:
        void CreateStaticCube(GameObject* object);
        void CreateStaticSphere(GameObject* object);
        void CreateDynamicCube(GameObject* object);
        void CreateDynamicSphere(GameObject* object);
        void CreateDynamicCapsule(GameObject* object);
        void LockDynamicBody(GameObject* object, bool x, bool y, bool z);
        void InitPhysics();
        void InitScene(Scene *scene, int sphereIndex);
        virtual void SetTransform(int index, Transform transform);
        void UpdatePhysics(f32 deltaTime, Scene *scene, ResourcesManager m);

        ~Physx()
        {
            if (cudaContextManager)
                cudaContextManager->release();
        };
    };

}
