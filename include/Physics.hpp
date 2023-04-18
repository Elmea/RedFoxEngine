#include <PhysX/PxConfig.h>
#include <PhysX/PxPhysicsAPI.h>

#include "GameObject.hpp"
#include "Scene.hpp"
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
    physx::PxControllerManager* m_manager; 
    physx::PxMaterial* material = nullptr;
    physx::PxPvd* pvd = nullptr;
    physx::PxCudaContextManager* cudaContextManager = nullptr;
  public:
    physx::PxScene* m_scene = nullptr;
  private:
    void CreateCubeCollider(const physx::PxTransform& t, physx::PxU32 size, physx::PxReal halfExtent);
    void CreateSphereCollider(const physx::PxTransform& t, physx::PxReal radius);
  public:
    void InitPhysics(Scene scene, int sphereIndex);
    void UpdatePhysics(GameObject *p_gameObjects, int p_gameObjectCount, Input *input);
  ~Physx()
  {
    if (cudaContextManager)
      cudaContextManager->release();
  };
};

}
