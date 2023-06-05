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

UIBEHAVIOUR(UI)
{
    if (self->isPressed)
        printf("Pressed\n");
}

BEHAVIOUR(Cube)
{         
    if (self->mass == 1)
    {           
        self->Color.x = 1;
        self->Color.y = 0;
        self->Color.z = 0;
    }
    else if (self->mass == 2)
    {
        self->Color.x = 0;
        self->Color.y = 1;
        self->Color.z = 0;
    }
    else if (self->mass == 4)
    {
        self->Color.x = 0;
        self->Color.y = 0;
        self->Color.z = 1;
    }
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
        physx::PxVec3 origin = { view.mat[0][3] + front.x, view.mat[1][3] + front.y, view.mat[2][3] +front.z};
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
                        printf("cube hit\n");
                        
                        if (storedCube == nullptr)
                        {
                            storedCube = &scene->gameObjects[i];
                            storedCubeId = i;
                        }
                        else if (storedCubeId != i)
                        {
                            int temp = storedCube->mass;
                            RedFoxEngine::GameObject* hitCube = &scene->gameObjects[i];
                            storedCube->mass = hitCube->mass;
                            hitCube->mass = temp;
                            storedCube = nullptr;
                            printf("cube swapped\n");
                        }
                    }
                }
            }
        }
    }
}

void Grab(RedFoxEngine::GameObject* self, RedFoxEngine::Scene* scene, RedFoxEngine::Input* input, RedFoxEngine::Physx* physx)
{

}

static bool pressed(RedFoxEngine::Key key)
{
    return (key.isHold || key.isPressed);
}

BEHAVIOUR(Player)
{
    scene->m_gameCamera.position = self->position;

    static Float3 cameraRotation;
    cameraRotation += {(f32)inputs->mouseYDelta* deltaTime, (f32)inputs->mouseXDelta* deltaTime, 0};
    if (cameraRotation.x > M_PI_2 + deltaTime) cameraRotation.x = M_PI_2;
    if (cameraRotation.x < -M_PI_2) cameraRotation.x = -M_PI_2;
    scene->m_gameCamera.orientation = Quaternion::FromEuler(-cameraRotation.x, -cameraRotation.y, cameraRotation.z);
    self->orientation = Quaternion::FromEuler(-cameraRotation.x, -cameraRotation.y, 0);
    
    Float3 velocity(0, 0, 0);
    float speed = 1;
    if (pressed(inputs->W) || pressed(inputs->Up))
    {
        velocity.x -= speed * cosf(- cameraRotation.y - PI / 2);
        velocity.z += speed * sinf(- cameraRotation.y - PI / 2);
    }
    if (pressed(inputs->S) || pressed(inputs->Down))
    {
        velocity.x += speed * cosf(- cameraRotation.y - PI / 2);
        velocity.z -= speed * sinf(- cameraRotation.y - PI / 2);
    }
    if (pressed(inputs->D) || pressed(inputs->Right))
    {
        velocity.x += speed * cosf(- cameraRotation.y);
        velocity.z -= speed * sinf(- cameraRotation.y);
    }
    if (pressed(inputs->A) || pressed(inputs->Left))
    {
        velocity.x -= speed * cosf(- cameraRotation.y);
        velocity.z += speed * sinf(- cameraRotation.y);
    }

    if (pressed(inputs->W) || pressed(inputs->S) || pressed(inputs->D) || pressed(inputs->A))
    {
        physx::PxRigidDynamic* playerCapsule = self->body->is<physx::PxRigidDynamic>();
        if (playerCapsule)
        {
            playerCapsule->addForce({ velocity.x, velocity.y, velocity.z }, physx::PxForceMode::eVELOCITY_CHANGE);
        }
    }

    if (pressed(inputs->Spacebar))
    {
        physx::PxRigidDynamic* playerCapsule = self->body->is<physx::PxRigidDynamic>();
        if (playerCapsule)
        {
            playerCapsule->addForce({ 0, 50000, 0 }, physx::PxForceMode::eFORCE);
        }
    }

    if (pressed(inputs->E))
    {
        Grab(self, scene, inputs, physx);
    }
    
    Shoot(self, scene, inputs, physx);
}

__declspec(dllexport) STARTGAME(StartGame)
{
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
    //printf("Started the game successfully!\n"); 
}

__declspec(dllexport) UPDATEGAME(UpdateGame)
{
/* 
    C++ by default exports functions by name mangeling  
    Meaning that our function UpdateGame will look like @UpdateGame%int etc...
    To stop that, we use a macro to export the function as the thing we named it
 */
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)

    RedFoxEngine::Scene *scene = (RedFoxEngine::Scene *)s;
    RedFoxEngine::Physx *physx = (RedFoxEngine::Physx *)p;
    RedFoxEngine::Input* inputs = (RedFoxEngine::Input*)i;
    RedFoxEngine::GameObject* player = &scene->gameObjects[2];

    static bool reloaded;
    if (!scene->isInit || reloaded == false)
    {
        player->behaviourIndex = scene->AddGameObjectBehaviour("Player", Player);
        player->UpdateTransform();

        // This UI object must be initialized in editor before playing
        scene->AddGameObjectBehaviour("Cube", Cube);
        scene->AddGameObjectBehaviour("Gun", Gun);


        scene->isInit = true;
        reloaded = true;
    }
}
