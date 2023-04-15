#include <PhysX/PxConfig.h>
#include <PhysX/PxPhysicsAPI.h>

#include "GameObject.hpp"

namespace RedFoxEngine
{

class Physx
{
  private:
    physx::PxDefaultAllocator allocator;
    physx::PxDefaultErrorCallback errorCallback;
    physx::PxFoundation* foundation = nullptr;
    physx::PxPhysics* physics = nullptr;
    physx::PxDefaultCpuDispatcher* dispatcher = nullptr;
    physx::PxScene* scene = nullptr;
    physx::PxMaterial* material = nullptr;
    physx::PxPvd* pvd = nullptr;
    physx::PxCudaContextManager* cudaContextManager = nullptr;
  private:
    void CreateCubeCollider(const physx::PxTransform& t, physx::PxU32 size, physx::PxReal halfExtent);
    void CreateSphereCollider(const physx::PxTransform& t, physx::PxReal radius);
  public:
    void InitPhysics(GameObject *p_gameObjects, int p_gameObjectCount, Model *sphere);
    void UpdatePhysics(GameObject *p_gameObjects, int p_gameObjectCount);
  ~Physx()
  {
    if (cudaContextManager)
      cudaContextManager->release();
  };
};

}
