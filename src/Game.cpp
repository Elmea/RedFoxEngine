#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Win32Platform.hpp"


#define REDFOXMATHS_IMPLEMENTATION
#include "RedfoxMaths.hpp"

#include "Physics.hpp"
#include "Scene.hpp"

using namespace RedFoxMaths;

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

static Float3 RotateVectorByQuaternion(Quaternion q, Float3 test)
{
    Float4 result = {test, 1};
    result = q.GetRotationMatrix() * result;
    return result.GetXYZF3();
}


__declspec(dllexport) UPDATEGAME(UpdateGame)
{
/* 
    C++ by default exports functions by name mangeling 
    Meaning that our function UpdateGame will look like @UpdateGame%int etc...
    To stop that, we use a macro to export the function as the thing we named it
 */
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)

    RedFoxEngine::Physx *physx = (RedFoxEngine::Physx *)p;
    RedFoxEngine::Scene *scene = (RedFoxEngine::Scene *)s;
    RedFoxEngine::GameObject *gameObjects = scene->gameObjects;
    int gameObjectCount = scene->gameObjectCount;
    gameObjects[0].position =
    {
        0, -11, 0
    };
    gameObjects[0].orientation =
    {
        1, 0, 0, 0
    };
    gameObjects[0].scale =
    {
        10000, 2, 10000
    };
    physx::PxRigidActor **actors = physx->actors;
    if (physx->actorCount)
    {
        if (!scene->isPaused)
            physx->m_scene->fetchResults(true);
        physx::PxRigidActor *player = actors[1];
        physx::PxTransform transform;
        
        transform = player->is<physx::PxRigidDynamic>()->getGlobalPose();
        transform.q.w = 1;
        transform.q.x = transform.q.y = transform.q.z = 0;
        player->is<physx::PxRigidDynamic>()->setGlobalPose(transform);

        int j = 1;
        for (int i = 1; i < (int)gameObjectCount && j < physx->actorCount; i++)
        {
            // actors[i]->is<physx::PxRigidDynamic>()->setGlobalPose(transform);
            physx::PxRigidDynamic *actor = actors[j]->is<physx::PxRigidDynamic>();
            if (actor)
            {
                if (!scene->isPaused && !actor->isSleeping())
                {
                    transform = actor->getGlobalPose();
                    gameObjects[i].position.x    = transform.p.x;
                    gameObjects[i].position.y    = transform.p.y;
                    gameObjects[i].position.z    = transform.p.z;
                    gameObjects[i].orientation.a = transform.q.w;
                    gameObjects[i].orientation.b = transform.q.x;
                    gameObjects[i].orientation.c = transform.q.y;
                    gameObjects[i].orientation.d = transform.q.z;
                    j++;
                }
                else
                {
                    transform.p.x = gameObjects[i].position.x   ;
                    transform.p.y = gameObjects[i].position.y   ;
                    transform.p.z = gameObjects[i].position.z   ;
                    transform.q.w = gameObjects[i].orientation.a;
                    transform.q.x = gameObjects[i].orientation.b;
                    transform.q.y = gameObjects[i].orientation.c;
                    transform.q.z	= gameObjects[i].orientation.d;
                    actor->setGlobalPose(transform);
                    j++;
                }
            }
    }
    scene->m_gameCamera.position = {gameObjects[1].position.x, gameObjects[1].position.y, gameObjects[1].position.z};
    static Float3 cameraRotation;
    cameraRotation += {(f32)input.mouseYDelta * deltaTime, (f32)input.mouseXDelta * deltaTime, 0}; 
    scene->m_gameCamera.orientation = Quaternion::FromEuler(-cameraRotation.x, -cameraRotation.y, cameraRotation.z);
        
    Float3 inputDirection(0, 0, 0);
    if (input.W || input.Up)    inputDirection.z += -1;
    if (input.S || input.Down)  inputDirection.z +=  1;
    if (input.A || input.Left)  inputDirection.x += -1;
    if (input.D || input.Right) inputDirection.x +=  1;
    if (input.W || input.S || input.A || input.D)
    {
        inputDirection = (Mat4::GetRotationY(-cameraRotation.y) * Mat4::GetRotationX(-cameraRotation.x) * inputDirection).GetXYZF3();
        inputDirection.Normalize();
        inputDirection = inputDirection * 200.f;
        physx::PxRigidActor *player = actors[1];
        Quaternion orientation = {1, 0, 0, 0};
        float speed = 5;
        physx::PxRigidDynamic *tPlayer = player->is<physx::PxRigidDynamic>();
        physx::PxVec3 velocity;
        if (tPlayer)
        {
            velocity = tPlayer->getLinearVelocity();

            velocity.x = speed * deltaTime * inputDirection.x;
            velocity.y = speed * deltaTime * inputDirection.y;
            //velocity.z = speed * deltaTime * inputDirection.z;
            tPlayer->setLinearVelocity(velocity);
            physx::PxTransform transform;
            // transform = tPlayer->getGlobalPose();
        }
    }
}
//for (int i = 0; i < (int)gameObjectCount; i++) // TODO physics code here ?
//{
//    gameObjects[i].position += Float3(sinf(time), cosf(time), 0) * 0.001f;        
//}
}
