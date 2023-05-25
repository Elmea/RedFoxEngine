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
        physx::PxMaterial* staticMaterial;
        physx::PxMaterial* dynamicMaterial;
        physx::PxPvd* pvd;
        physx::PxCudaContextManager* cudaContextManager;
    public:
        physx::PxScene* m_scene = nullptr;
        int actorCount = 0;
    private:
    public:
        void CreateStaticCube(GameObject* object, Transform transform);
        void CreateStaticSphere(GameObject* object, Transform transform);
        void CreateDynamicCube(GameObject* object, Transform transform);
        void CreateDynamicSphere(GameObject* object, Transform transform);
        void CreateDynamicCapsule(GameObject* object, Transform transform);
        void LockDynamicBody(GameObject* object, bool x, bool y, bool z);
        void InitPhysics();
        void InitScene(Scene *scene, int sphereIndex, int cubeIndex);
        void UpdatePhysics(f32 deltaTime, Scene *scene, ResourcesManager m);

        ~Physx()
        {
            if (cudaContextManager)
                cudaContextManager->release();
        };
    };

}
