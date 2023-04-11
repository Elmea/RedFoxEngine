#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Win32Platform.hpp"

#include "Transform.hpp"

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
    /*
    Quaternion alpha = Quaternion::FromEuler(Float3(0, 90, 0));
    Quaternion beta = Quaternion::FromEuler(Float3(0, 0, 90));
    for (int i = 0; i < (int)gameObjectCount; i++) // TODO physics code here ?
    {
         gameObjects[i].position += Float3(sinf(time), cosf(time), 0) * 0.001f;
         gameObjects[i].orientation = Quaternion::SLerp(beta, alpha, time);
         gameObjects[i].scale = Misc::Lerp(0.1, 2, Misc::Abs(cosf(time)));

    }
    */
    for (int i = 0; i < (int)gameObjectCount; i++)
    {
        
        gameObjects[i].Test();
//        gameObjects[i].rb.setPos
      
       
        /*
        RedFoxEngine::Transform tr;       
        gameObjects[i].rb->setGlobalPose(PxTransform(PxVec3(0,0,0)));
        tr.assign(gameObjects[i].rb->getGlobalPose());
        
        printf("%f, %f, %f\n", tr.position.x, tr.position.y, tr.position.z);
        */
    }



}