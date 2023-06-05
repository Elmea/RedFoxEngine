#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define NOMINMAX
#include <windows.h>
#endif
#include "Win32Platform.hpp"


#define REDFOXMATHS_IMPLEMENTATION
#include "RedfoxMaths.hpp"

#include "Physics.hpp"
#include "Scene.hpp"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

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

BEHAVIOUR(Gun)
{
    self->position;
}

void setColorFromMass(RedFoxMaths::Float3* color, int mass)
{
    if (mass == 1)
    {
        color->x = 1;
        color->y = 0;
        color->z = 0;
    }
    else if (mass == 2)
    {
        color->x = 0;
        color->y = 1;
        color->z = 0;
    }
    else if (mass == 4)
    {
        color->x = 0;
        color->y = 0;
        color->z = 1;
    }
    else
    {
        color->x = 1;
        color->y = 1;
        color->z = 1;
    }

}

UIBEHAVIOUR(UI)
{
}

BEHAVIOUR(Cube)
{         
    setColorFromMass(&self->Color, self->mass);
}

static RedFoxEngine::GameObject* storedCube = nullptr;
static int storedCubeId;

void Shoot(RedFoxEngine::GameObject* self, RedFoxEngine::Scene* scene, RedFoxEngine::Input* input, RedFoxEngine::Physx* physx)
{
    if (input->mouseLClick.isPressed)
    {
        RedFoxMaths::Float3 ray_ndc = {
            0,0, 1
        };
        RedFoxMaths::Float4 ray_clip = { ray_ndc.x, ray_ndc.y, -1, 1 };
        RedFoxMaths::Float4 ray_eye = scene->m_gameCamera.m_projection.GetInverseMatrix() * ray_clip;
        ray_eye = { ray_eye.x, ray_eye.y, -1, 0 };
        RedFoxMaths::Float4 ray_world = scene->m_gameCamera.GetViewMatrix().GetInverseMatrix() * ray_eye;
        ray_world.Normalize();
        RedFoxMaths::Mat4 view = scene->m_gameCamera.GetViewMatrix().GetInverseMatrix();
        RedFoxMaths::Float3 front = ray_world.GetXYZF3();
        front.Normalize();
        front *= 1.2f;
        physx::PxVec3 origin = { view.mat[0][3] + front.x, view.mat[1][3] + front.y, view.mat[2][3] + front.z };
        physx::PxVec3 unitDir = { ray_world.x, ray_world.y, ray_world.z };
        physx::PxRaycastBuffer hitCalls;
        if (physx->m_scene->raycast(origin, unitDir, scene->m_gameCamera.m_parameters._far, hitCalls, physx::PxHitFlag::eANY_HIT))
        {
            physx::PxRaycastHit hit = hitCalls.getAnyHit(0);
            if (hit.actor)
            {

                for (int i = 0; i < scene->gameObjectCount; i++)
                {
                    if (scene->gameObjects[i].body == hit.actor
                        && scene->gameObjectBehaviours[scene->gameObjects[i].behaviourIndex].function == Cube)
                    {

                        if (storedCube == nullptr)
                        {
                            storedCube = &scene->gameObjects[i];
                            setColorFromMass(&scene->gameUIs[2].selectedColor, storedCube->mass);
                            storedCubeId = i;
                        }
                        else if (storedCubeId != i)
                        {
                            int temp = storedCube->mass;
                            RedFoxEngine::GameObject* hitCube = &scene->gameObjects[i];
                            storedCube->mass = hitCube->mass;
                            hitCube->mass = temp;
                            storedCube = nullptr;
                            setColorFromMass(&scene->gameUIs[2].selectedColor, 0);
                        }
                        
                        break;
                    }
                }
            }
        }
    }

    if (input->mouseRClick.isPressed)
    {
        storedCube = nullptr;
        setColorFromMass(&scene->gameUIs[2].selectedColor, 0);
    }
    
}

static RedFoxEngine::GameObject* objectHold = nullptr;

void Grab(RedFoxEngine::GameObject* self, RedFoxEngine::Scene* scene, RedFoxEngine::Input* input, RedFoxEngine::Physx* physx)
{
    RedFoxMaths::Float3 ray_ndc = { 0, 0, 1 };
    RedFoxMaths::Float4 ray_clip = { ray_ndc.x, ray_ndc.y, -1, 1 };
    RedFoxMaths::Float4 ray_eye = scene->m_gameCamera.m_projection.GetInverseMatrix() * ray_clip;
    ray_eye = { ray_eye.x, ray_eye.y, -1, 0 };
    RedFoxMaths::Float4 ray_world = scene->m_gameCamera.GetViewMatrix().GetInverseMatrix() * ray_eye;
    ray_world.Normalize();
    RedFoxMaths::Mat4 view = scene->m_gameCamera.GetViewMatrix().GetInverseMatrix();
    RedFoxMaths::Float3 front = ray_world.GetXYZF3();
    front.Normalize();
    front *= 1.2f;
    physx::PxVec3 origin = { view.mat[0][3] + front.x, view.mat[1][3] + front.y, view.mat[2][3] + front.z };
    physx::PxVec3 unitDir = { ray_world.x, ray_world.y, ray_world.z };
    physx::PxRaycastBuffer hitCalls;
    if (physx->m_scene->raycast(origin, unitDir, 5, hitCalls, physx::PxHitFlag::eANY_HIT))
    {
        physx::PxRaycastHit hit = hitCalls.getAnyHit(0);
        if (hit.actor)
        {
            for (int i = 0; i < scene->gameObjectCount; i++)
            {
                if (scene->gameObjects[i].body == hit.actor
                    && scene->gameObjectBehaviours[scene->gameObjects[i].behaviourIndex].function == Cube)
                {
                    objectHold = &scene->gameObjects[i];

                    break;
                }
            }
        }
    }
}

static Float3 cameraRotation;

void Throw(RedFoxEngine::GameObject* self, RedFoxEngine::Scene* scene, RedFoxEngine::Input* input, RedFoxEngine::Physx* physx)
{
    if (objectHold)
    {
        physx::PxRigidDynamic* cube = objectHold->body->is<physx::PxRigidDynamic>();

        float force = 500000;
        physx::PxVec3 forceVec = { cosf(cameraRotation.y + PI / 2.f) * force, sinf(cameraRotation.x + PI/4.f) * (force) , sinf(cameraRotation.y - PI / 2.f) * force };
        cube->addForce(forceVec);

        objectHold = nullptr;
    }
}

static bool pressed(RedFoxEngine::Key key)
{
    return (key.isHold || key.isPressed);
}


void Jump(RedFoxEngine::GameObject* self, RedFoxEngine::Input* inputs)
{

    if (pressed(inputs->Spacebar))
    {
        physx::PxRigidDynamic* playerCapsule = self->body->is<physx::PxRigidDynamic>();
        if (playerCapsule)
        {
            if(playerCapsule->getLinearVelocity().magnitude() < 2000)
                playerCapsule->addForce({ 0, 2000, 0 }, physx::PxForceMode::eFORCE);
        }
    }

}

BEHAVIOUR(Player)
{
    scene->m_gameCamera.position = self->position;
    scene->m_gameCamera.position.y += 1.5f;

    cameraRotation -= {(f32)inputs->mouseYDelta* deltaTime, (f32)inputs->mouseXDelta* deltaTime, 0};

    if (cameraRotation.x > M_PI_2) cameraRotation.x = M_PI_2;
    if (cameraRotation.x < -M_PI_2) cameraRotation.x = -M_PI_2;

    if (cameraRotation.y > PI * 2) cameraRotation.y = 0;
    if (cameraRotation.y < 0) cameraRotation.y = PI * 2;

    scene->m_gameCamera.orientation = Quaternion::FromEuler(cameraRotation.x, cameraRotation.y, cameraRotation.z);
    self->orientation = Quaternion::FromEuler(cameraRotation.x, cameraRotation.y, 0);
    
    if (objectHold)
    {
        objectHold->position = RedFoxMaths::Float3{ self->position.x + cosf(cameraRotation.y + PI /2 ) * 1.5f ,  self->position.y + 3 , self->position.z + sinf(-cameraRotation.y - PI / 2) * 1.5f };
        physx::PxTransform transform{ physx::PxVec3{objectHold->position.x, objectHold->position.y, objectHold->position.z} };
        objectHold->body->setGlobalPose(transform);
    }

    Float3 velocity(0, 0, 0);
    float speed = 0.5f;
    if (pressed(inputs->W) || pressed(inputs->Up))
    {
        velocity.x -= speed * cosf(cameraRotation.y - PI / 2);
        velocity.z += speed * sinf(cameraRotation.y - PI / 2);    
    }
    if (pressed(inputs->S) || pressed(inputs->Down))
    {
        velocity.x += speed * cosf(cameraRotation.y - PI / 2);
        velocity.z -= speed * sinf(cameraRotation.y - PI / 2);
    }
    if (pressed(inputs->D) || pressed(inputs->Right))
    {
        velocity.x += speed * cosf(cameraRotation.y);
        velocity.z -= speed * sinf(cameraRotation.y);
    }
    if (pressed(inputs->A) || pressed(inputs->Left))
    {
        velocity.x -= speed * cosf(cameraRotation.y);
        velocity.z += speed * sinf(cameraRotation.y);
    }

    if (pressed(inputs->W) || pressed(inputs->S) || pressed(inputs->D) || pressed(inputs->A))
    {
        physx::PxRigidDynamic* playerCapsule = self->body->is<physx::PxRigidDynamic>();
        if (playerCapsule)
        {
            playerCapsule->addForce({ velocity.x, velocity.y, velocity.z }, physx::PxForceMode::eVELOCITY_CHANGE);
        }
    }

    Jump(self, inputs);
    if (pressed(inputs->E))
    {
        Grab(self, scene, inputs, physx);
    }

    if (inputs->Q.isPressed)
    {
        Throw(self, scene, inputs, physx);
    }
    
    Shoot(self, scene, inputs, physx);
}


__declspec(dllexport) STARTGAME(StartGame)
{
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}

__declspec(dllexport) UPDATEGAME(UpdateGame)
{

#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)

    RedFoxEngine::Scene *scene = (RedFoxEngine::Scene *)s;
    RedFoxEngine::Physx *physx = (RedFoxEngine::Physx *)p;
    RedFoxEngine::Input* inputs = (RedFoxEngine::Input*)i;
    RedFoxEngine::GameObject* player = &scene->gameObjects[2];

    static bool reloaded;
    if (!scene->isInit || reloaded == false)
    {
        physx::PxRigidDynamic* playerCapsule = player->body->is<physx::PxRigidDynamic>();
        if (playerCapsule)
        {
            playerCapsule->setLinearDamping(2);
            playerCapsule->setMass(50);
        }

        player->behaviourIndex = scene->AddGameObjectBehaviour("Player", Player);
        player->UpdateTransform();

        scene->AddGameObjectBehaviour("Cube", Cube);
        scene->AddGameObjectBehaviour("Gun", Gun);
        player->modelIndex = -1;

        scene->isInit = true;
        reloaded = true;
    }
}
