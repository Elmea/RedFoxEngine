#include <thread>
#include <vector>

#include "Engine.hpp"
#include "PhysX/PxRigidBody.h"

using namespace RedFoxEngine;
using namespace physx;

void Physx::CreateCubeCollider(RedFoxMaths::Float3 position, RedFoxMaths::Float3 scale)
{
	PxTransform t(position.x, position.y, position.z);
	PxShape* shape = physics->createShape(PxBoxGeometry(scale.x, scale.y, scale.z), *material);
	PxRigidDynamic* body = physics->createRigidDynamic(t);
	body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
	m_scene->addActor(*body);
	shape->release();
}

void Physx::CreateSphereCollider(RedFoxMaths::Float3 position, PxReal radius)
{
	PxTransform t(position.x, position.y, position.z);
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

	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	pvd = PxCreatePvd(*foundation);
	pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale(), true, pvd);

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
	m_scene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 1);
	m_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1);

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

	for (u32 i = 1; i < (u32)scene.gameObjectCount; i++)
	{
		if (scene.gameObjects[i].modelIndex == sphereIndex)
			CreateSphereCollider(scene.gameObjects[i].position, scene.gameObjects[i].scale.x);
		else
			CreateCubeCollider(scene.gameObjects[i].position, scene.gameObjects[i].scale);
	}
}

void Physx::SetTransform(int index, Transform transform)
{
	int temp = m_scene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC);
	if (index < temp)
	{
		PxActor *actor;
		m_scene->getActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC, (physx::PxActor**)&actor, 1, index);
		if (actor && actor->is<physx::PxRigidDynamic>())
		{
			physx::PxTransform t;
			t.p = {transform.position.x, transform.position.y, transform.position.z};
			t.q = {transform.orientation.b, transform.orientation.c, transform.orientation.d, transform.orientation.a};
			actor->is<physx::PxRigidDynamic>()->setGlobalPose(t);
		}
	}
}
void Physx::UpdatePhysics(f32 deltaTime, ResourcesManager m, bool isPaused)
{
	int temp = m_scene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC);
	if (temp)
	{
		if(!isPaused)
			m_scene->simulate(deltaTime);
		
		actorCount = temp;
		actors = (PxRigidActor **)m.TemporaryAllocation(sizeof(actors) * actorCount);
		m_scene->getActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC, (physx::PxActor**)actors, actorCount);
	}
}