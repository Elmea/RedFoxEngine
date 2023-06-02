/* date = February 22nd 2023 11:17 am */
#pragma once

#include <stddef.h>
#include <stdint.h>
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

struct WindowDimension
{
    int width;
    int height;
};

struct Key
{
    u8 isPressed : 1;
    u8 isHold    : 1;
};

class Input
{
    bool hideCursor;

public:
    bool lockMouse = false;
    int mouseXPosition;
    int mouseYPosition;
    int mouseXDelta;
    int mouseYDelta;
    Key  mouseLClick;
    Key  mouseRClick;
    Key Q;
    Key W;
    Key E;
    Key R;
    Key T;
    Key Y;
    Key U;
    Key I;
    Key O;
    Key P;
    Key OpenBracket;
    Key CloseBracket;
    Key BackSlash;
    Key A;
    Key S;
    Key D;
    Key F;
    Key G;
    Key H;
    Key J;
    Key K;
    Key L;
    Key SemiColon;
    Key Apostrophe;
    Key Tilda;
    Key LShift;
    Key Enter;
    Key LControl;
    Key Escape;
    Key Z;
    Key X;
    Key C;
    Key V;
    Key B;
    Key N;
    Key M;
    Key Comma;
    Key Period;
    Key Slash;
    Key RShift;
    Key Spacebar;
    Key Up;
    Key Down;
    Key Left;
    Key Right;
    Key Delete;
    void HideCursor(bool state)
    {
        if (state != hideCursor)
        {
            hideCursor = state;
            ShowCursor(!state);  
        }
    }
};

#define UPDATEGAME(name) void name(void *s,  \
                                   void *p,  \
                                   void* i,  \
                                   float deltaTime)
typedef UPDATEGAME(_updategame);

#define STARTGAME(name) void name(void *s,  \
                                   void *p,  \
                                   void* i)
typedef STARTGAME(_startgame);

struct GameLibrary
{
    HINSTANCE library;
    FILETIME lastTime;
    _updategame* update;
    _startgame* start;
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
    int FileExist(const char *file);
    void FatalError(const char *message);
    void MessageProcessing(Input *input);
    WindowDimension GetWindowDimension();
    void SetMousePosition(Input *input, int x, int y);
    static u64 GetTimerFrequency();
    static f64 GetTimer();
    static void WaitForThread(HANDLE thread);
    void SwapFramebuffers();
    GameLibrary LoadGameLibrary(const char* startFunction, const char* updateFunction, const char *libraryPath, GameLibrary game);
};

} // namespace RedFoxEngine

#define STR2(x) #x
#define STR(x) STR2(x)

#ifdef _DEBUG
#define Assert(cond)                                                                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(cond))                                                                                                   \
            __debugbreak();                                                                                            \
    } while (0)
#else
#define Assert(cond)                                                                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
    } while (0)
#endif
