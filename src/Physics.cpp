#include "Engine.hpp"

#include <thread>

using namespace RedFoxEngine;
using namespace physx;

void Engine::createCubeCollider(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	PxShape* shape = m_physics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *m_material);
	PxRigidDynamic* body = m_physics->createRigidDynamic(t);
	body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
	m_scene->addActor(*body);
	shape->release();
}

void Engine::createSphereCollider(const PxTransform& t, PxReal radius)
{
	PxShape* shape = m_physics->createShape(PxSphereGeometry(radius), *m_material);
	PxRigidDynamic* body = m_physics->createRigidDynamic(t);
	body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
	m_scene->addActor(*body);
	shape->release();
}

void Engine::InitPhysics()
{
	m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_errorCallback);

	m_pvd = PxCreatePvd(*m_foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, PxTolerancesScale());

	// TODO(a.perche): Make GPU work
	//PxCudaContextManagerDesc cudaContextManagerDesc;
	//m_cudaContextManager = PxCreateCudaContextManager(*m_foundation, cudaContextManagerDesc, PxGetProfilerCallback());

	PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	m_dispatcher = PxDefaultCpuDispatcherCreate(std::thread::hardware_concurrency());
	sceneDesc.cpuDispatcher = m_dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	//sceneDesc.cudaContextManager = m_cudaContextManager;
	//sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
	//sceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;
	m_scene = m_physics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = m_scene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	m_material = m_physics->createMaterial(0.5f, 0.5f, 0.6f);

	for (u32 i = 0; i < m_gameObjectCount; i++)
	{
		PxTransform gameObjectTransform(m_gameObjects[i].position.x,
			m_gameObjects[i].position.y, m_gameObjects[i].position.z);
		if (isSphere(m_gameObjects[i]))
			createSphereCollider(gameObjectTransform, (float)m_gameObjects[i].radius);
		else
			createCubeCollider(gameObjectTransform, 1, 0.5);
	}
	
	PxRigidStatic* groundPlane = PxCreatePlane(*m_physics, PxPlane(0, 1, 0, 10), *m_material);
	m_scene->addActor(*groundPlane);
}

void Engine::UpdatePhysics()
{
	PxU32 nbActors = m_scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	if (nbActors)
	{
		m_scene->simulate(1.0f / 60.0f);
		m_scene->fetchResults(true);
		std::vector<physx::PxRigidActor*> actors;
		actors.resize(m_gameObjectCount + 1); // We add the hard coded floor
		m_scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
		for (int i = 1; i < m_gameObjectCount; i++)
		{
			PxTransform transform = actors[i]->getGlobalPose();
			m_gameObjects[i].position.x = transform.p.x;
			m_gameObjects[i].position.y = transform.p.y;
			m_gameObjects[i].position.z = transform.p.z;
			m_gameObjects[i].orientation.a = transform.q.w;
			m_gameObjects[i].orientation.b = transform.q.x;
			m_gameObjects[i].orientation.c = transform.q.y;
			m_gameObjects[i].orientation.d = transform.q.z;
		}
	}
}

bool Engine::isCube(GameObject& object)
{
	return object.model == &m_models[0];
}

bool Engine::isSphere(GameObject& object)
{
	return object.model == &m_models[1];
}
