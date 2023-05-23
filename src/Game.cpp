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

UIBEHAVIOUR(UI)
{
    if (self->isPressed)
        printf("Pressed\n");
}

BEHAVIOUR(Gun)
{
    if (input->mouseLClick)
    {
        printf("Clic");
    }
}

BEHAVIOUR(Player)
{
    scene->m_gameCamera.position = self->position;

    static Float3 cameraRotation;
    cameraRotation += {(f32)input->mouseYDelta* deltaTime, (f32)input->mouseXDelta* deltaTime, 0};
    if (cameraRotation.x > M_PI_2 + deltaTime) cameraRotation.x = M_PI_2;
    if (cameraRotation.x < -M_PI_2) cameraRotation.x = -M_PI_2;
    scene->m_gameCamera.orientation = Quaternion::FromEuler(-cameraRotation.x, -cameraRotation.y, cameraRotation.z);
    
    Float3 inputDirection(0, 0, 0);
    float speed = 5.f;
    if (input->W || input->Up)    inputDirection.z += -1;
    if (input->S || input->Down)  inputDirection.z += 1;
    if (input->A || input->Left)  inputDirection.x += -1;
    if (input->D || input->Right) inputDirection.x += 1;
    
    Float3 velocity(0, 0, 0);
    if (input->W || input->S || input->A || input->D)
    {
        inputDirection = (Mat4::GetRotationY(-cameraRotation.y) * Mat4::GetRotationX(-cameraRotation.x) * inputDirection).GetXYZF3();
        inputDirection.Normalize();
        velocity.x = speed * deltaTime * inputDirection.x;
        velocity.y = 0;
        velocity.z = speed * deltaTime * inputDirection.z;
        physx::PxRigidDynamic* playerCapsule = self->body->is<physx::PxRigidDynamic>();
        if (playerCapsule)
        {
            playerCapsule->addForce({ velocity.x, velocity.y, velocity.z }, physx::PxForceMode::eVELOCITY_CHANGE);
        }
    }
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

    if (!scene->isInit)
    {
        // Problem with that is this is not reflected in the editor UI at runtime, for both gameobject and gameUI
        RedFoxEngine::GameObject* player = &scene->gameObjects[2];
        player->behaviourIndex = scene->AddGameObjectBehaviour("Player", Player);
        player->UpdateTransform();
        
        // This UI object must be initialized in editor before playing
        scene->gameUIs[1].behaviourIndex = scene->AddUIBehaviour("UI", UI);

        player->behaviourIndex = scene->AddGameObjectBehaviour("Gun", Gun);

        scene->isInit = true;
    }
}
