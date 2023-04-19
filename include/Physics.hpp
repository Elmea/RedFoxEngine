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
    physx::PxFoundation *foundation = nullptr;
    physx::PxPhysics *physics = nullptr;
    physx::PxDefaultCpuDispatcher *dispatcher = nullptr;
    physx::PxMaterial *material = nullptr;
    physx::PxPvd *pvd = nullptr;
    physx::PxCudaContextManager *cudaContextManager = nullptr;
  public:
    physx::PxScene *m_scene = nullptr;
    physx::PxRigidActor **actors;
    int actorCount = 0;
  private:
  public:
    void CreateCubeCollider(RedFoxMaths::Float3 position, physx::PxU32 size, physx::PxReal halfExtent);
    void CreateSphereCollider(RedFoxMaths::Float3 position, physx::PxReal radius);
    void InitPhysics(Scene scene, int sphereIndex);
    void UpdatePhysics(f32 deltaTime, ResourcesManager m, bool isPaused);

  ~Physx()
  {
    if (cudaContextManager)
      cudaContextManager->release();
  };
};

}
