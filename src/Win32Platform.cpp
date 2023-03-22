#include "Win32Platform.hpp"
#include "glcorearb.h"
#include "imgui_impl_win32.h"
using namespace RedFoxEngine;

int Platform::m_running;

static void FatalError(const char *message)
{
    MessageBoxA(nullptr, message, "Error", MB_ICONEXCLAMATION);
    ExitProcess(0);
}

static int StringisEqual(const char *src, const char *dst, size_t dstlen)
{
    while (*src && dstlen-- && *dst)
    {
        if (*src++ != *dst++)
            return 0;
    }
    return (dstlen && *src == *dst) || (!dstlen && *src == 0);
}

#define Assert(cond)                                                                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(cond))                                                                                                   \
            __debugbreak();                                                                                            \
    } while (0)

#ifndef NDEBUG
static void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                   const GLchar *message, const void *user)
{
    if (severity != GL_DEBUG_SEVERITY_NOTIFICATION && severity != GL_DEBUG_SEVERITY_LOW)
    {
        OutputDebugStringA(message);
        OutputDebugStringA("\n");
    }
    if (severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM)
    {
        if (IsDebuggerPresent())
        {
            Assert(!"OpenGL error - check the callstack in debugger");
        }
        FatalError("OpenGL API usage error! Use debugger to examine call stack!");
    }
}
#endif

PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

#define X(type, name) type name;
GL_FUNCTIONS(X)
#undef X

static HGLRC Win32InitOpenGL(HWND window)
{
    int Attrib[] = {
        WGL_DRAW_TO_WINDOW_ARB,
        GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB,
        GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB,
        GL_TRUE,
        WGL_PIXEL_TYPE_ARB,
        WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB,
        24,
        WGL_DEPTH_BITS_ARB,
        24,
        WGL_STENCIL_BITS_ARB,
        8,

        // uncomment for sRGB framebuffer, from WGL_ARB_framebuffer_sRGB extension
        // https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_framebuffer_sRGB.txt
        WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB,
        GL_TRUE,

        // uncomment for multisampeld framebuffer, from WGL_ARB_multisample extension
        // https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_multisample.txt
        // WGL_SAMPLE_BUFFERS_ARB, 1,
        WGL_SAMPLES_ARB,
        4, // 4x MSAA

        0,
    };

    HDC WindowDC = GetDC(window);

    int format;
    unsigned int formats;

    if (!wglChoosePixelFormatARB(WindowDC, Attrib, nullptr, 1, &format, &formats) || formats == 0)
    {
        FatalError("OpenGL does not support required pixel format!");
    }
    PIXELFORMATDESCRIPTOR desc = {};
    desc.nSize = sizeof(desc);
    int ok = DescribePixelFormat(WindowDC, format, sizeof(desc), &desc);
    Assert(ok && "Failed to describe OpenGL pixel format");

    if (!SetPixelFormat(WindowDC, format, &desc))
    {
        FatalError("Cannot set OpenGL selected pixel format!");
    }

    HGLRC OpenGLRC = 0;
    {
        int attrib[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB,
            4,
            WGL_CONTEXT_MINOR_VERSION_ARB,
            5,
            WGL_CONTEXT_PROFILE_MASK_ARB,
            WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#ifndef NDEBUG
            // ask for debug context for non "Release" builds
            // this is so we can enable debug callback
            WGL_CONTEXT_FLAGS_ARB,
            WGL_CONTEXT_DEBUG_BIT_ARB,
#endif
            0,
        };

        OpenGLRC = wglCreateContextAttribsARB(WindowDC, nullptr, attrib);
        if (!OpenGLRC)
        {
            FatalError("Cannot create modern OpenGL context! OpenGL version 4.5 not supported?");
        }

        BOOL ok = wglMakeCurrent(WindowDC, OpenGLRC);
        Assert(ok && "Failed to make current OpenGL context");

        // load OpenGL funcs
#define X(type, name)                                                                                                  \
    name = (type)wglGetProcAddress(#name);                                                                             \
    Assert(name);
        GL_FUNCTIONS(X)
#undef X

#ifndef NDEBUG
        // enable debug callback
        glDebugMessageCallback(&DebugCallback, nullptr);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif
    }
    ReleaseDC(window, WindowDC);
    return (OpenGLRC);
}

Platform::Platform(int width, int height)
{
    GetWglFunctions();
    m_running = 1;

    // Get instance from Windows
    GetModuleHandleExA(0, nullptr, &m_applicationContext);
    // Create Window
    m_window = CreateRedFoxWindow(width, height);
    m_glContext = Win32InitOpenGL(m_window);
    ShowWindow(m_window, 5);
    HCURSOR hCurs1;

    hCurs1 = LoadCursor(nullptr, IDC_ARROW);
    SetCursor(hCurs1);
    GetWindowDimension();
}

WindowDimension Platform::GetWindowDimension()
{
    RECT ClientRect;
    GetClientRect(m_window, &ClientRect);
    m_windowDimension.width = ClientRect.right - ClientRect.left;
    m_windowDimension.height = ClientRect.bottom - ClientRect.top;
    glViewport(0, 0, m_windowDimension.width, m_windowDimension.height);
    return (m_windowDimension);
}

void Platform::MessageProcessing(Input *input)
{
    MSG Message;
    while (PeekMessageA(&Message, nullptr, 0, 0, PM_REMOVE))
    {
        switch (Message.message)
        {

        // NOTE This message gets called on window resize
        case WM_SIZE: {
            GetWindowDimension();
        }
        break;
        case WM_MOUSEMOVE: {
            input->mouseXPosition = LOWORD(Message.lParam);
            input->mouseYPosition = HIWORD(Message.lParam);
        }
        break;
        case WM_MOUSEWHEEL: {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }
        break;

        case WM_LBUTTONDOWN: {
            input->mouseLClick = true;
            SetCapture(m_window);
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }
        break;
        case WM_LBUTTONUP:
        {
            input->mouseLClick = false;
            ReleaseCapture();
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }break;
        case WM_RBUTTONDOWN: {
            input->mouseRClick = true;
            SetCapture(m_window);
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }
        break;
        case WM_RBUTTONUP:
        {
            input->mouseRClick = false;
            ReleaseCapture();
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }
        break;

        case WM_KEYUP:
        case WM_KEYDOWN: {
            TranslateMessage(&Message);
            DispatchMessage(&Message);

            int VKCode = (int)Message.wParam;
            // NOTE: variable names explain the bit shift
            // int WasDown = ((Message.lParam & (1 << 30)) != 0);
            int IsDown = ((Message.lParam & (1 << 31)) == 0);

            // NOTE(V. Caraulan): This function maps the Virtual Keys of the keyboard, to physical keys
            UINT scanCode = MapVirtualKeyEx(VKCode, MAPVK_VK_TO_VSC, GetKeyboardLayout(0));

            // NOTE(V. Caraulan): the comments are for US/ISO international keys
            // https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
            switch (scanCode)
            {
            case 0x01: // Escape
            {
            }
            break;
            case 0x10: {
                input->Q = IsDown;
            }
            break; // Q
            case 0x11: {
                input->W = IsDown;
            }
            break; // W
            case 0x12: {
                input->E = IsDown;
            }
            break;     // E
            case 0x13: // R
            case 0x14: // T
            case 0x15: // Y

            case 0x16: // U
                break;
            case 0x17: {
                input->I = IsDown;
            }
            break;     // I
            case 0x18: // O
            case 0x19: // P
            case 0x1A: // [
            case 0x1B: // ]
            case 0x1C: // Enter / Return
            case 0x1D: // LCtrl
                break;

            case 0x1E: {
                input->A = IsDown;
            }
            break; // A
            case 0x1F: {
                input->S = IsDown;
            }
            break; // S
            case 0x20: {
                input->D = IsDown;
            }
            break;     // D
            case 0x21: // F
            case 0x22: // G
            case 0x23: // H
                break;
            case 0x24: {
                input->J = IsDown;
            }
            break; // J
            case 0x25: {
                input->K = IsDown;
            }
            break; // K
            case 0x26: {
                input->L = IsDown;
            }
            break;
            case 0x27: // ;
            case 0x28: // '
            case 0x29: // `~
            case 0x2A: // LShift
            case 0x2B: // \|

            case 0x2C: // Z
            case 0x2D: // X
            case 0x2E: // C
            case 0x2F: // V
            case 0x30: // B
            case 0x31: // N
            case 0x32: // M
            case 0x33: // <
            case 0x34: // >
            case 0x35: // /?
            case 0x36: // RShift
                break;
            // 37 (Keypad-*) or (*/PrtScn) on a 83/84-key keyboard
            // 38 (LAlt),
            case 0x39: {
            }
            break; // Space bar
                // 3a (CapsLock)
                // 3b (F1), 3c (F2), 3d (F3), 3e (F4), 3f (F5), 40 (F6), 41 (F7), 42 (F8), 43 (F9), 44 (F10)
                break;
            default: {
            }
            }
        }
        break;
        default: {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }
        }
    }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static LRESULT CALLBACK MainWindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    if (ImGui_ImplWin32_WndProcHandler(Window, Message, WParam, LParam))
            return 1;
    LRESULT Result = 0;
    switch (Message)
    {
    case WM_DESTROY: {
        Platform::m_running = 0;
        PostQuitMessage(0);
    }
    break;

    case WM_QUIT: {
        Platform::m_running = 0;
        DestroyWindow(Window);
    }
    break;

    case WM_CLOSE: {
        Platform::m_running = 0;
    }
    break;

    default: {
        Result = DefWindowProc(Window, Message, WParam, LParam);
    }
    }
    return (Result);
}

u64 Platform::GetTimerFrequency(void)
{
    LARGE_INTEGER Result;
    QueryPerformanceFrequency(&Result);
    return Result.QuadPart;
}

u64 Platform::GetTimer(void)
{
    LARGE_INTEGER Result;
    QueryPerformanceCounter(&Result);
    return Result.QuadPart;
}


UPDATEGAME(updateStub)
{
//NOTE We use an empty function in case our library loading fails, so we don't crash
}

_updategame *Platform::LoadGameLibrary(const char *functionName, const char *libraryPath, HINSTANCE &gameLibrary,
    LPFILETIME LastWriteTime, _updategame *functionPointer)
{
    FILETIME temp = *LastWriteTime;
    /*
     * NOTE: On Windows the lowest overhead function to open a file handle
     * is the misnamed function CreateFile.
     * We get a file handle, check the last write time, and if the library
     * on disk is newer, we free ours and load the newest one.
     *
     * Another complication of this function is the fact that when a library is loaded,
     * it cannot be overwritten (by compiling). So we always copy the library, and load the
     * copy, that way the original can always be overwritten, which is what we want to hot reload.
    */ 
    HANDLE File = CreateFileA("game.dll",                         // PATH 
                              GENERIC_READ,                       // Desired access
                              FILE_SHARE_WRITE | FILE_SHARE_READ, // Share Mode
                              nullptr,                               // Security Attributes
                              OPEN_EXISTING,                      // Creation Disposition
                              FILE_ATTRIBUTE_NORMAL,              // Flags and attributes
                              nullptr);                              // Template file
    GetFileTime(File, nullptr, nullptr, LastWriteTime);
    CloseHandle(File);
    if (CompareFileTime(&temp, LastWriteTime) != 0)
    {
        if (gameLibrary)
            FreeLibrary(gameLibrary);
        CopyFileA(libraryPath, "gameCopy.dll", false);
        gameLibrary = LoadLibraryA("gameCopy.dll");
        if (gameLibrary)
            functionPointer = (_updategame *)GetProcAddress(gameLibrary, functionName);
        if (functionPointer == nullptr)
            functionPointer = &updateStub;
    }
    return ((_updategame*)functionPointer);
}

Window Platform::CreateRedFoxWindow(int Width, int Height)
{
    Window window = nullptr;
    WNDCLASS WindowClass = {};
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = MainWindowCallback;
    WindowClass.hInstance = m_applicationContext;
    WindowClass.lpszClassName = "RedFoxEngineWindowClass";

    if (RegisterClassA(&WindowClass))
    {
        window = CreateWindowExA(WindowClass.style,            // Optional window styles.
                                 WindowClass.lpszClassName,    // Window class
                                 "RedFox Engine",              // Window text
                                 WS_OVERLAPPEDWINDOW,          // Window style
                                 CW_USEDEFAULT, CW_USEDEFAULT, // Position and Size
                                 Width, Height,
                                 nullptr,                  // Parent window
                                 nullptr,                  // Menu
                                 WindowClass.hInstance, // Instance handle
                                 nullptr);                 // Additional application data
    }
    return (window);
}

void Platform::FatalError(const char *message)
{
    MessageBoxA(nullptr, message, "Error", MB_ICONEXCLAMATION);
    ExitProcess(0);
}

void Platform::GetWglFunctions(void)
{
    // to get WGL funcs we need valid GL context, so create dummy window for dummy GL contetx
    HWND dummy = CreateWindowExW(0, L"STATIC", L"DummyWindow", WS_OVERLAPPED, CW_USEDEFAULT, CW_USEDEFAULT,
                                 CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, nullptr, nullptr);

    if (!dummy && "Failed to create dummy window")
    {
        __debugbreak();
        return;
    }
    HDC dc = GetDC(dummy);
    if (!dc && "Failed to get device context for dummy window")
    {
        __debugbreak();
        return;
    }
    PIXELFORMATDESCRIPTOR desc = {};
    desc.nSize = sizeof(desc);
    desc.nVersion = 1;
    desc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    desc.iPixelType = PFD_TYPE_RGBA;
    desc.cColorBits = 24;

    int format = ChoosePixelFormat(dc, &desc);
    if (!format)
    {
        return; //"Cannot choose OpenGL pixel format for dummy window!");
    }

    int ok = DescribePixelFormat(dc, format, sizeof(desc), &desc);
    if (!ok && "Failed to describe OpenGL pixel format")
    {
        __debugbreak();
        return;
    }

    // reason to create dummy window is that SetPixelFormat can be called only once for the window
    if (!SetPixelFormat(dc, format, &desc))
    {
        {
            __debugbreak();
            return;
        } // FatalError("Cannot set OpenGL pixel format for dummy window!");
    }

    HGLRC rc = wglCreateContext(dc);
    if (!rc && "Failed to create OpenGL context for dummy window")
    {
        __debugbreak();
        return;
    }

    ok = wglMakeCurrent(dc, rc);
    if (!ok && "Failed to make current OpenGL context for dummy window")
    {
        __debugbreak();
        return;
    }

    // https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_extensions_string.txt
    PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB =
        (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
    if (!wglGetExtensionsStringARB)
    {
        {
            __debugbreak();
            return;
        } //("OpenGL does not support WGL_ARB_extensions_string extension!");
    }

    const char *ext = wglGetExtensionsStringARB(dc);
    if (!ext && "Failed to get OpenGL WGL extension string")
    {
        __debugbreak();
        return;
    }

    const char *start = ext;
    for (;;)
    {
        while (*ext != 0 && *ext != ' ')
        {
            ext++;
        }

        size_t length = ext - start;
        if (StringisEqual("WGL_ARB_pixel_format", start, length))
        {
            // https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_pixel_format.txt
            wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
        }
        else if (StringisEqual("WGL_ARB_create_context", start, length))
        {
            // https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_create_context.txt
            wglCreateContextAttribsARB =
                (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
        }
        else if (StringisEqual("WGL_EXT_swap_control", start, length))
        {
            // https://www.khronos.org/registry/OpenGL/extensions/EXT/WGL_EXT_swap_control.txt
            wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
        }

        if (*ext == 0)
        {
            break;
        }

        ext++;
        start = ext;
    }

    if (!wglChoosePixelFormatARB || !wglCreateContextAttribsARB || !wglSwapIntervalEXT)
    {
        FatalError("OpenGL does not support required WGL extensions for modern context!");
    }

    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(rc);
    ReleaseDC(dummy, dc);
    DestroyWindow(dummy);
}