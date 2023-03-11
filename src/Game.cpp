#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Win32Platform.hpp"


#define REDFOXMATHS_IMPLEMENTATION
#include "RedfoxMaths.hpp"

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

__declspec(dllexport) UPDATEGAME(UpdateGame)
{
    #pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
    static Float3 modelPosition(0, 0, 0);
    Float3 inputDirection(0, 0, 0);

    static Float3 speed;

    if (input.I)
        modelPosition.y += deltaTime;
    if (input.K)
        modelPosition.y += -deltaTime;
    if (input.J)
        modelPosition.x += -deltaTime;
    if (input.L)
        modelPosition.x += deltaTime;
    if (input.W)
        inputDirection.z += 1;
    if (input.S)
        inputDirection.z += -1;
    if (input.A)
        inputDirection.x += 1;
    if (input.D)
        inputDirection.x += -1;
    inputDirection =
        (Mat4::GetRotationY(-cameraRotation.y) * Mat4::GetRotationX(-cameraRotation.x) * inputDirection).GetXYZF3();
    inputDirection.Normalize();
    inputDirection = inputDirection * 10.f;
    *cameraPosition += speed * (f32)deltaTime + inputDirection * ((f32)deltaTime * (f32)deltaTime * 0.5f);
    speed += inputDirection * (f32)deltaTime * 0.5f;
    speed *= 0.9f; // drag

    for (int i = 0; i < (int)gameObjectCount; i++) // TODO physics code here ?
    {
        if (gameObjects[i].parent == nullptr)
        {
            gameObjects[i].position += Float3(sinf(time), cosf(time), 0) * 0.001f;
            gameObjects[i].orientation =  Quaternion::SLerp({ 1,0.2f,0.2f,0.2f }, { -1,0.8f,0.8f,0.8f }, time);
            gameObjects[i].scale = 1;
        }
    }
}