#include <thread>
#include <vector>

#include "Engine.hpp"
#include "PhysX/PxRigidBody.h"

using namespace RedFoxEngine;
using namespace physx;

void Physx::CreateStaticCube(GameObject* object, Transform transform)
{
	PxQuat q(transform.orientation.b, transform.orientation.c, transform.orientation.d, transform.orientation.a);
	PxTransform t(transform.position.x, transform.position.y, transform.position.z, q);
	if (transform.scale.x < 1) transform.scale.x = 1;
	if (transform.scale.y < 1) transform.scale.y = 1;
	if (transform.scale.z < 1) transform.scale.z = 1;
	PxShape* shape = physics->createShape(PxBoxGeometry(transform.scale.x, transform.scale.y, transform.scale.z), *staticMaterial);
	object->body = physics->createRigidStatic(t);
	object->body->attachShape(*shape);
	m_scene->addActor(*object->body);
	shape->release();
}

void Physx::CreateStaticSphere(GameObject* object, Transform transform)
{
	PxReal radius = object->scale.x;
	PxQuat q(transform.orientation.b, transform.orientation.c, transform.orientation.d, transform.orientation.a);
	PxTransform t(transform.position.x, transform.position.y, transform.position.z, q);
	PxShape* shape = physics->createShape(PxSphereGeometry(radius), *staticMaterial);
	object->body = physics->createRigidStatic(t);
	object->body->attachShape(*shape);
	m_scene->addActor(*object->body);
	shape->release();
}

void Physx::CreateDynamicCube(GameObject* object, Transform transform, float mass)
{
	PxQuat q(transform.orientation.b, transform.orientation.c, transform.orientation.d, transform.orientation.a);
	PxTransform t(transform.position.x, transform.position.y, transform.position.z, q);
	if (transform.scale.x < 1) transform.scale.x = 1;
	if (transform.scale.y < 1) transform.scale.y = 1;
	if (transform.scale.z < 1) transform.scale.z = 1;
	PxShape* shape = physics->createShape(PxBoxGeometry(transform.scale.x, transform.scale.y, transform.scale.z), *dynamicMaterial);
	object->body = physics->createRigidDynamic(t);
	object->body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*object->body->is<PxRigidBody>(), 10.0f);
	m_scene->addActor(*object->body);
	shape->release();
}

void Physx::CreateDynamicSphere(GameObject* object, Transform transform, float mass)
{
	PxReal radius = transform.scale.x;
	PxTransform t(transform.position.x, transform.position.y, transform.position.z);
	PxShape* shape = physics->createShape(PxSphereGeometry(radius), *dynamicMaterial);
	object->body = physics->createRigidDynamic(t);
	object->body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*object->body->is<PxRigidBody>(), 10.0f);
	m_scene->addActor(*object->body);
	shape->release();
}

void Physx::CreateDynamicCapsule(GameObject* object, Transform transform, float mass)
{
	PxQuat q(transform.orientation.b, transform.orientation.c, transform.orientation.d, transform.orientation.a);
	PxTransform t(transform.position.x, transform.position.y, transform.position.z, q);
	object->body = physics->createRigidDynamic(t);
	PxTransform relativePose(PxQuat(PxHalfPi, PxVec3(0, 0, 1)));
	PxShape* shape = physics->createShape(PxCapsuleGeometry(transform.scale.x, transform.scale.y), *dynamicMaterial);
	shape->setLocalPose(relativePose);
	object->body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*object->body->is<PxRigidBody>(), mass);
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

void Physx::InitScene(Scene *scene, int sphereIndex, int cubeIndex)
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

	staticMaterial = physics->createMaterial(1.5f, 0.5f, 0.01f);
	dynamicMaterial = physics->createMaterial(1.5f, 0.5f, 0.01f);
	GameObject* player = &scene->gameObjects[2];
	Transform playerTransform = { { player->position.x, player->position.y, player->position.z },
								{ player->scale.x, player->scale.y, player->scale.z },
								RedFoxMaths::Quaternion(1,0,0,0) };
	CreateDynamicCapsule(player, playerTransform);
	LockDynamicBody(player, true, true, true);

	for (u32 i = 1; i < (u32)scene->gameObjectCount; i++)
	{
			if (i == 2)
				continue;
			if (scene->gameObjects[i].modelIndex == sphereIndex)
				CreateStaticSphere(&scene->gameObjects[i], scene->GetWorldTransform(i));
			else if(scene->gameObjects[i].modelIndex == cubeIndex)
				CreateStaticCube(&scene->gameObjects[i], scene->GetWorldTransform(i));
			else
				scene->gameObjects[i].body = nullptr;
	}
}

void Physx::UpdatePhysics(f32 deltaTime, Scene* scene, ResourcesManager m)
{
	static bool wasPaused;
	actorCount = m_scene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC);
	if (actorCount)
	{
		m_scene->simulate(deltaTime);
		m_scene->fetchResults(true);

		for (int i = 1; i < (int)scene->gameObjectCount; i++)
		{
			if (scene->gameObjects[i].body)
			{
				physx::PxRigidDynamic* dynamicActor = scene->gameObjects[i].body->is<physx::PxRigidDynamic>();
				if (dynamicActor)
				{
					if (!scene->isPaused)
					{
						wasPaused = true;
						dynamicActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
						physx::PxTransform transform = dynamicActor->getGlobalPose();
						RedFoxMaths::Quaternion q = { transform.q.w, transform.q.x, transform.q.y, transform.q.z };
						RedFoxMaths::Float3 p = { transform.p.x, transform.p.y, transform.p.z };
						Transform t = { p, scene->gameObjects[i].scale, q };
						scene->gameObjects[i].transform = scene->GetWorldTransformFromLocal(t, i);
					}
					else
					{
						dynamicActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
					}
					if (wasPaused)
					{
						dynamicActor->addForce({ 0,0,0 });
						wasPaused = false;
					}
				}
			}
		}

	}
}
