#include <thread>
#include <vector>

#include "Engine.hpp"
#include "PhysX/PxRigidBody.h"

using namespace RedFoxEngine;
using namespace physx;

void Physx::CreateStaticCube(GameObject* object, Transform transform)
{
	PxQuat q(object->orientation.b, object->orientation.c, object->orientation.d, object->orientation.a);
	PxTransform t(transform.position.x, transform.position.y, transform.position.z, q);
	PxShape* shape = physics->createShape(PxBoxGeometry(object->scale.x, object->scale.y, object->scale.z), *material);
	object->body = physics->createRigidStatic(t);
	object->body->attachShape(*shape);
	m_scene->addActor(*object->body);
	shape->release();
}

void Physx::CreateStaticSphere(GameObject* object, Transform transform)
{
	PxReal radius = object->scale.x;
	PxQuat q(object->orientation.b, object->orientation.c, object->orientation.d, object->orientation.a);
	PxTransform t(transform.position.x, transform.position.y, transform.position.z, q);
	PxShape* shape = physics->createShape(PxSphereGeometry(radius), *material);
	object->body = physics->createRigidStatic(t);
	object->body->attachShape(*shape);
	m_scene->addActor(*object->body);
	shape->release();
}

void Physx::CreateDynamicCube(GameObject* object, Transform transform)
{
	PxQuat q(object->orientation.b, object->orientation.c, object->orientation.d, object->orientation.a);
	PxTransform t(transform.position.x, transform.position.y, transform.position.z, q);
	PxShape* shape = physics->createShape(PxBoxGeometry(object->scale.x, object->scale.y, object->scale.z), *material);
	object->body = physics->createRigidDynamic(t);
	object->body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*object->body->is<PxRigidBody>(), 10.0f);
	m_scene->addActor(*object->body);
	shape->release();
}

void Physx::CreateDynamicSphere(GameObject* object, Transform transform)
{
	PxReal radius = object->scale.x;
	PxTransform t(transform.position.x, transform.position.y, transform.position.z);
	PxShape* shape = physics->createShape(PxSphereGeometry(radius), *material);
	object->body = physics->createRigidDynamic(t);
	object->body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*object->body->is<PxRigidBody>(), 10.0f);
	m_scene->addActor(*object->body);
	shape->release();
}

void Physx::CreateDynamicCapsule(GameObject* object, Transform transform)
{
	PxQuat q(object->orientation.b, object->orientation.c, object->orientation.d, object->orientation.a);
	PxTransform t(transform.position.x, transform.position.y, transform.position.z, q);
	PxShape* shape = physics->createShape(PxCapsuleGeometry(object->scale.x, object->scale.y), *material);
	object->body = physics->createRigidDynamic(t);
	object->body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*object->body->is<PxRigidBody>(), 10.0f);
	m_scene->addActor(*object->body);
	shape->release();
}

void Physx::LockDynamicBody(GameObject* object, bool x, bool y, bool z)
{
	PxRigidDynamic* body = object->body->is<PxRigidDynamic>();
	body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, x);
	body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, y);
	body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, z);
}

void Physx::InitPhysics()
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
	dispatcher = PxDefaultCpuDispatcherCreate(std::thread::hardware_concurrency());
}

void Physx::InitScene(Scene *scene, int sphereIndex)
{
	if  (m_scene != nullptr)
	{
		m_scene->release();
	}
	PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);

	sceneDesc.cpuDispatcher = dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;	
	m_scene = physics->createScene(sceneDesc);
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
	
	RedFoxEngine::GameObject* player = &scene->gameObjects[2];
	player->orientation = RedFoxMaths::Quaternion({ 90.f, 0.f, 0.f });
	CreateDynamicCapsule(player, scene->GetWorldTransform(2));
	LockDynamicBody(player, true, false, true);

	// If commented, the game code moving the player capsule crashes
	for (u32 i = 1; i < (u32)scene->gameObjectCount; i++)
	{
		if (i != 2)
		{
			if (scene->gameObjects[i].modelIndex == sphereIndex)
				CreateStaticSphere(&scene->gameObjects[i], scene->GetWorldTransform(i));
			else
				CreateStaticCube(&scene->gameObjects[i], scene->GetWorldTransform(i));
		}
	}
}

void Physx::SetTransform(int index, Transform transform)
{
	int temp = m_scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	if (index < temp)
	{
		PxActor* actor;
		m_scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, (PxActor**)&actor, 1, index);
		if (actor)
		{
			PxTransform t;
			t.p = { transform.position.x, transform.position.y, transform.position.z };
			t.q = { transform.orientation.b, transform.orientation.c, transform.orientation.d, transform.orientation.a };
			actor->is<PxRigidActor>()->setGlobalPose(t);
		}
	}
}

void Physx::UpdatePhysics(f32 deltaTime, Scene* scene, ResourcesManager m)
{
	actorCount = m_scene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC);
	if (actorCount)
	{
		if (!scene->isPaused)
		{
			m_scene->simulate(deltaTime);
			m_scene->fetchResults(true);
		}
		physx::PxTransform transform;
		for (int i = 1; i < (int)scene->gameObjectCount; i++)
		{
			if (scene->gameObjects[i].body)
			{
				physx::PxRigidDynamic* dynamicActor = scene->gameObjects[i].body->is<physx::PxRigidDynamic>();
				if (dynamicActor && !dynamicActor->isSleeping())
				{
					transform = dynamicActor->getGlobalPose();

					scene->gameObjects[i].transform = scene->GetLocalTransformFromParent(i);
				}
			}
		}
	}
}
