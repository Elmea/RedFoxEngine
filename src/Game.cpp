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

void Gun(RedFoxEngine::GameObject* self, RedFoxEngine::Scene* scene, RedFoxEngine::Input* input, RedFoxEngine::Physx* physx)
{
    if (input->mouseLClick)
    {
        printf("Clic\n");
        RedFoxMaths::Float4 ray_clip = { 0, 0, -1, 1 };
        RedFoxMaths::Float4 ray_eye = scene->m_gameCamera.m_projection.GetInverseMatrix() * ray_clip;
        ray_eye = { ray_eye.x, ray_eye.y, -1, 0 };        

        RedFoxMaths::Mat4 transformMatrix = RedFoxMaths::Mat4::CreateTransformMatrix(scene->m_gameCamera.position, scene->m_gameCamera.orientation, scene->m_gameCamera.scale);
        RedFoxMaths::Float4 ray_world = transformMatrix.GetInverseMatrix() * ray_eye;
      
        ray_world.Normalize();
        transformMatrix = RedFoxMaths::Mat4::CreateTransformMatrix(scene->m_gameCamera.position, scene->m_gameCamera.orientation, scene->m_gameCamera.scale);
        RedFoxMaths::Mat4 view = transformMatrix.GetInverseMatrix();
        physx::PxVec3 unitDir = { ray_world.x, ray_world.y, ray_world.z };
        physx::PxVec3 origin = { view.mat[0][3], view.mat[1][3], view.mat[2][3] };
        physx::PxRaycastBuffer hitCalls;
        if (physx->m_scene->raycast(origin, unitDir, scene->m_gameCamera.m_parameters._far, hitCalls, physx::PxHitFlag::eANY_HIT))
        {
            physx::PxRaycastHit hit = hitCalls.getAnyHit(0);
        } 
    }
}

BEHAVIOUR(Cube)
{
    //printf("cube\n");
}

BEHAVIOUR(Player)
{
    scene->m_gameCamera.position = self->position;

    static Float3 cameraRotation;
    cameraRotation += {(f32)inputs->mouseYDelta* deltaTime, (f32)inputs->mouseXDelta* deltaTime, 0};
    if (cameraRotation.x > M_PI_2 + deltaTime) cameraRotation.x = M_PI_2;
    if (cameraRotation.x < -M_PI_2) cameraRotation.x = -M_PI_2;
    scene->m_gameCamera.orientation = Quaternion::FromEuler(-cameraRotation.x, -cameraRotation.y, cameraRotation.z);
    
    Float3 inputDirection(0, 0, 0);
    if (inputs->W || inputs->Up)    inputDirection.z += -1;
    if (inputs->S || inputs->Down)  inputDirection.z += 1;
    if (inputs->A || inputs->Left)  inputDirection.x += -1;
    if (inputs->D || inputs->Right) inputDirection.x += 1;
    
    float speed = 5000000;
    Float3 velocity(0, 0, 0);
    if (inputs->W || inputs->S || inputs->A || inputs->D)
    {
        inputDirection = (Mat4::GetRotationY(-cameraRotation.y) * Mat4::GetRotationX(-cameraRotation.x) * inputDirection).GetXYZF3();
        inputDirection.Normalize();
        velocity.x = speed * deltaTime * inputDirection.x;
        velocity.y = 0;
        velocity.z = speed * deltaTime * inputDirection.z;
        physx::PxRigidDynamic* playerCapsule = self->body->is<physx::PxRigidDynamic>();
        if (playerCapsule)
        {
            playerCapsule->addForce({ velocity.x, velocity.y, velocity.z }, physx::PxForceMode::eFORCE);
        }
    }
    
    Gun(self, scene, inputs, physx);
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


        scene->isInit = true;
        reloaded = true;
    }
}
