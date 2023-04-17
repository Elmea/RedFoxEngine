#include <thread>
#include <vector>

#include "Engine.hpp"

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
	material = physics->createMaterial(0.5f, 0.5f, 0.6f);

	// TODO: Replace with scene loaded context
	PxRigidStatic* groundPlane = PxCreatePlane(*physics, PxPlane(0, 1, 0, 10), *material);
	m_scene->addActor(*groundPlane);

	for (u32 i = 0; i < (u32)scene.gameObjectCount; i++)
	{
		PxTransform gameObjectTransform(scene.gameObjects[i].position.x, scene.gameObjects[i].position.y, scene.gameObjects[i].position.z);
		if (scene.gameObjects[i].modelIndex == sphereIndex)
			CreateSphereCollider(gameObjectTransform, (float)scene.gameObjects[i].radius);
		else
			CreateCubeCollider(gameObjectTransform, 1, 0.5);
	}
}

void Physx::UpdatePhysics(GameObject *p_gameObjects, int p_gameObjectCount)
{
	PxU32 nbActors = m_scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	if (nbActors)
	{
		m_scene->simulate(1.0f / 60.0f);
		m_scene->fetchResults(true);
		std::vector<physx::PxRigidActor*> actors;
		actors.resize(nbActors);
		m_scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
		for (int i = 1; i < p_gameObjectCount; i++)
		{
			PxTransform transform = actors[i]->getGlobalPose();
			p_gameObjects[i].position.x = transform.p.x;
			p_gameObjects[i].position.y = transform.p.y;
			p_gameObjects[i].position.z = transform.p.z;
			p_gameObjects[i].orientation.a = transform.q.w;
			p_gameObjects[i].orientation.b = transform.q.x;
			p_gameObjects[i].orientation.c = transform.q.y;
			p_gameObjects[i].orientation.d = transform.q.z;
		}
	}
}