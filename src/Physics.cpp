#include <thread>
#include <vector>

#include "Engine.hpp"
#include "PhysX/PxRigidBody.h"

using namespace RedFoxEngine;
using namespace physx;

void Physx::CreateCubeCollider(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	PxShape* shape = physics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *material);
	PxRigidDynamic* body = physics->createRigidDynamic(t);
	body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
	m_scene->addActor(*body);
	shape->release();
}

void Physx::CreateSphereCollider(const PxTransform& t, PxReal radius)
{
	PxShape* shape = physics->createShape(PxSphereGeometry(radius), *material);
	PxRigidDynamic* body = physics->createRigidDynamic(t);
	body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
	m_scene->addActor(*body);
	shape->release();
}

void Physx::InitPhysics(Scene scene, int sphereIndex)
{
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, errorCallback);

	pvd = PxCreatePvd(*foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale());

	PxCudaContextManagerDesc cudaContextManagerDesc;
	cudaContextManager = PxCreateCudaContextManager(*foundation, cudaContextManagerDesc, PxGetProfilerCallback());
	
	PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -30.f /*-9.81f*/, 0.0f);

	dispatcher = PxDefaultCpuDispatcherCreate(std::thread::hardware_concurrency());
	sceneDesc.cpuDispatcher = dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	if (cudaContextManager->contextIsValid())
	{
		sceneDesc.cudaContextManager = cudaContextManager;
		sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
		sceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;
	}
	m_scene = physics->createScene(sceneDesc);
	
	PxPvdSceneClient* pvdClient = m_scene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	material = physics->createMaterial(0.5f, 0.5f, 0.1f);

	// TODO: Replace with scene loaded context
	PxRigidStatic* groundPlane = PxCreatePlane(*physics, PxPlane(0, 1, 0, 10), *material);
	m_scene->addActor(*groundPlane);

	for (u32 i = 0; i < (u32)scene.gameObjectCount; i++)
	{
		PxTransform gameObjectTransform(scene.gameObjects[i].position.x, scene.gameObjects[i].position.y, scene.gameObjects[i].position.z);
		if (scene.gameObjects[i].modelIndex == sphereIndex)
			CreateSphereCollider(gameObjectTransform, (float)0.5);
		else
			CreateCubeCollider(gameObjectTransform, 1, 0.5);
	}
}

void Physx::UpdatePhysics(f32 deltaTime, ResourcesManager m)
{
	int temp = m_scene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC);
	if (temp)
	{
		m_scene->simulate(deltaTime);
		actorCount = temp;			
		actors = (PxRigidActor **)m.TemporaryAllocation(sizeof(actors) * actorCount);
		m_scene->getActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC, (physx::PxActor**)actors, actorCount);
	}
}