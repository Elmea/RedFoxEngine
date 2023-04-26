/* date = February 22nd 2023 11:17 am */
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <new>
#include <stddef.h>
#include <stdint.h>
#include <windows.h>

#include "OpenGLFunctions.hpp"
#include <GL/gl.h>
#include "Model.hpp"
#include "GameObject.hpp"

typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

namespace RedFoxEngine
{
typedef HWND Window;
typedef HINSTANCE ApplicationContext;

struct WindowOrigin
{
    int x;
    int y;
};

struct WindowDimension
{
    int width;
    int height;
};

struct Input
{
    bool lockMouse = false;
    int mouseXPosition;
    int mouseYPosition;
    int mouseXDelta;
    int mouseYDelta;
    u8  mouseLClick : 1;
    u8  mouseRClick : 1;

    u8 Q : 1;
    u8 W : 1;
    u8 E : 1;
    u8 R : 1;
    u8 T : 1;
    u8 Y : 1;
    u8 U : 1;
    u8 I : 1;
    u8 O : 1;
    u8 P : 1;
    u8 OpenBracket : 1;
    u8 CloseBracket : 1;
    u8 BackSlash : 1;
    u8 A : 1;
    u8 S : 1;
    u8 D : 1;
    u8 F : 1;
    u8 G : 1;
    u8 H : 1;
    u8 J : 1;
    u8 K : 1;
    u8 L : 1;
    u8 SemiColon : 1;
    u8 Apostrophe : 1;
    u8 Tilda : 1;
    u8 LShift : 1;
    u8 Enter : 1;
    u8 LControl : 1;
    u8 Escape : 1;
    u8 Z : 1;
    u8 X : 1;
    u8 C : 1;
    u8 V : 1;
    u8 B : 1;
    u8 N : 1;
    u8 M : 1;
    u8 Comma : 1;
    u8 Period : 1;
    u8 Slash : 1;
    u8 RShift : 1;
    u8 Spacebar : 1;
    u8 Up : 1;
    u8 Down : 1;
    u8 Left : 1;
    u8 Right : 1;
    u8 Delete : 1;
};

#ifndef UPDATEGAME
#define UPDATEGAME(name) void name(void *s, \
                                   void *p, RedFoxEngine::Input input, float deltaTime)
#endif
typedef UPDATEGAME(_updategame);

struct GameLibrary
{
    HINSTANCE library;
    FILETIME lastTime;
    _updategame *update;
};

class Platform
{

private:
    ApplicationContext m_applicationContext;

public:
    HGLRC m_glContext;
    RECT m_minimizedDimension;
    WindowDimension m_windowDimension;
    Window m_window;
    static int m_running;

private:
    Window CreateRedFoxWindow(int width, int height);
    void GetWglFunctions();
public:
    Platform() = default;
    Platform(int width, int height);
    void Maximize();
    void FatalError(const char *message);
    void LockMouse(RedFoxEngine::Window m_window, int mouseX, int mouseY, RedFoxEngine::Input* input);
    void MessageProcessing(Input *input);
    WindowOrigin GetWindowOrigin();
    WindowDimension GetWindowDimension();
    void SetMousePosition(int x, int y);
    static u64 GetTimerFrequency();
    static f64 GetTimer();
    void SwapFramebuffers();
    GameLibrary LoadGameLibrary(const char *functionName, const char *libraryPath, GameLibrary game);
};
} // namespace RedFoxEngine

#define STR2(x) #x
#define STR(x) STR2(x)

#define Assert(cond)                                                                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(cond))                                                                                                   \
            __debugbreak();                                                                                            \
    } while (0)