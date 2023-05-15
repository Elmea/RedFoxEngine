#include <thread>
#include <vector>

#include "Engine.hpp"
#include "PhysX/PxRigidBody.h"

using namespace RedFoxEngine;
using namespace physx;

void Physx::CreateCubeCollider(GameObject* object, PxU32 size, PxReal halfExtent)
{
	PxTransform t(object->position.x, object->position.y, object->position.z);
	PxShape* shape = physics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *material);
	object->body = physics->createRigidDynamic(t);
	object->body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*object->body, 10.0f);
	m_scene->addActor(*object->body);
	shape->release();
}

void Physx::CreateSphereCollider(GameObject* object)
{
	RedFoxMaths::Float3 position = object->position;
	PxReal radius = object->radius;
	PxTransform t(position.x, position.y, position.z);
	PxShape* shape = physics->createShape(PxSphereGeometry(radius), *material);
	object->body = physics->createRigidDynamic(t);
	object->body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*object->body, 10.0f);
	m_scene->addActor(*object->body);
	shape->release();
}

void Physx::CreatePlayerCollider(GameObject* object, PxF32 radius, PxF32 height, PxF32 contactOffset, PxF32 stepOffset, PxF32 maxJumpHeight)
{
	PxCapsuleControllerDesc capsuleControllerDesc = {};
	PxExtendedVec3 pos(object->position.x, object->position.y, object->position.z);
	capsuleControllerDesc.position = pos;
	capsuleControllerDesc.radius = radius;
	capsuleControllerDesc.height = height;
	capsuleControllerDesc.contactOffset = contactOffset;
	capsuleControllerDesc.stepOffset = stepOffset;
	capsuleControllerDesc.maxJumpHeight = maxJumpHeight;
	capsuleControllerDesc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
	object->controller = controllerManager->createController(capsuleControllerDesc);
}

void Physx::InitPhysics(Scene scene, int sphereIndex)
{
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, errorCallback);

#if _DEBUG
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	pvd = PxCreatePvd(*foundation);
	pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale(), true, pvd);
#else
	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale(), false);
#endif


	PxCudaContextManagerDesc cudaContextManagerDesc;
	cudaContextManager = PxCreateCudaContextManager(*foundation, cudaContextManagerDesc, PxGetProfilerCallback());

	PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -30.f /*-9.81f*/, 0.0f);

	dispatcher = PxDefaultCpuDispatcherCreate(std::thread::hardware_concurrency());
	sceneDesc.cpuDispatcher = dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	m_scene = physics->createScene(sceneDesc);
	controllerManager = PxCreateControllerManager(*m_scene);
	controllerManager->setPreciseSweeps(false); // Less precise but faster according to PhysX doc
	controllerManager->setOverlapRecoveryModule(true); // Move the CCT to a safe place

#if _DEBUG
	m_scene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 1);
	m_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1);

	PxPvdSceneClient* pvdClient = m_scene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
#endif

	material = physics->createMaterial(0.5f, 0.5f, 0.1f);

	PxRigidStatic* groundPlane = PxCreatePlane(*physics, PxPlane(0, 1, 0, 10), *material);
	m_scene->addActor(*groundPlane);

	CreatePlayerCollider(&scene.gameObjects[1], 1.f, 2.f, 1.f, 1.f, 1.f);
	/*
	for (u32 i = 2; i < (u32)scene.gameObjectCount; i++)
	{
		if (scene.gameObjects[i].modelIndex == sphereIndex)
			CreateSphereCollider(&scene.gameObjects[i]);
		else
			CreateCubeCollider(&scene.gameObjects[i], 1, 1);
	}
	*/
}

void Physx::SetTransform(int index, Transform transform)
{
	int temp = m_scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	if (index < temp)
	{
		PxActor* actor;
		m_scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, (PxActor**)&actor, 1, index);
		if (actor && actor->is<PxRigidDynamic>())
		{
			PxTransform t;
			t.p = { transform.position.x, transform.position.y, transform.position.z };
			t.q = { transform.orientation.b, transform.orientation.c, transform.orientation.d, transform.orientation.a };
			actor->is<PxRigidDynamic>()->setGlobalPose(t);
		}
	}
}
void Physx::UpdatePhysics(f32 deltaTime, ResourcesManager m, bool isPaused)
{
	actorCount = m_scene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC);
	if (actorCount)
	{
		if (!isPaused)
			m_scene->simulate(deltaTime);
		actors = (PxRigidActor**)m.TemporaryAllocation(sizeof(actors) * actorCount);
		m_scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, (PxActor**)actors, actorCount);
	}
}
