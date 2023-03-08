#include "Engine.hpp"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

#define REDFOXMATHS_IMPLEMENTATION
#include "RedfoxMaths.hpp"

using namespace RedFoxEngine;

void Engine::ObjModelPush(const char *path)
{
    int errorState = 0;
    if ((errorState = ParseModel(&m_models[m_modelCount++].obj, path)))
        __debugbreak();
}

Engine::Engine(int width, int height)
{
    memset(this, 0, sizeof(Engine)); // TODO formalize this in a C++ way
    m_arenaAllocator = InitVirtualMemory(1 * GigaByte);
    IncreaseTotalCapacity(&m_arenaAllocator, 1 * MegaByte);

    new (&m_platform) Platform(width, height);

    m_platform.GetWindowDimension();
    m_graphics.InitGraphics();

    m_models = (Model *)MyMalloc(&m_arenaAllocator, sizeof(Model) * 100);
    m_modelCount = 0;
    ObjModelPush("ts_bot912.obj");
   
    for (int i = 0; i < (int)m_modelCount; i++)
    {
        m_graphics.InitModel(&m_models[i]);
        m_models[i].parent = nullptr;
        // Tmp code for scene graph
        if (i > 0)
        {
            m_models[i].parent = &m_models[i - 1];
        }
        DeInitGraphicsObj(&m_models[i].obj);
    }
    m_models[1] = m_models[0];
    m_models[1].position.x += 2.f;
    m_modelCount++;
    QueryPerformanceFrequency((LARGE_INTEGER *)&m_frequency);
    QueryPerformanceCounter((LARGE_INTEGER *)&m_startingTime);
    m_input = {};
}

void Engine::GetInputs()
{
    m_time = Platform::GetTimer();

    int mouseX = m_input.mouseXPosition;
    int mouseY = m_input.mouseYPosition;
    m_platform.MessageProcessing(&m_input); // TODO(V. Caraulan): make a Input struct that handles all the inputs
    m_platform.GetWindowDimension();        // TODO(V. Caraulan): do this in message processing
    if (GetFocus() != m_platform.m_window)
    {
        m_input.mouseXDelta = m_input.mouseXPosition = 0;
        m_input.mouseYDelta = m_input.mouseYPosition = 0;
    }
    else
    {
        m_input.mouseXDelta = m_input.mouseXPosition - mouseX;
        m_input.mouseYDelta = m_input.mouseYPosition - mouseY;
    }
}

using namespace RedFoxMaths;

void Engine::Update()
{
    static Float3 modelPosition(0, 0, 0);
    static Float3 cameraPosition(0, 0, -4);
    static Float3 cameraRotation(0, 0, 0);
    Float3 inputDirection(0, 0, 0);

    static Float3 speed;

    if (m_input.I)
        modelPosition.y += m_deltaTime;
    if (m_input.K)
        modelPosition.y += -m_deltaTime;
    if (m_input.J)
        modelPosition.x += -m_deltaTime;
    if (m_input.L)
        modelPosition.x += m_deltaTime;
    if (m_input.W)
        inputDirection.z += 1;
    if (m_input.S)
        inputDirection.z += -1;
    if (m_input.A)
        inputDirection.x += 1;
    if (m_input.D)
        inputDirection.x += -1;

    inputDirection =
        (Mat4::GetRotationY(-cameraRotation.y) * Mat4::GetRotationX(-cameraRotation.x) * inputDirection).GetXYZF3();
    inputDirection.Normalize();
    inputDirection = inputDirection * 10.f;
    cameraPosition += speed * (f32)m_deltaTime + inputDirection * ((f32)m_deltaTime * (f32)m_deltaTime * 0.5f);
    speed += inputDirection * (f32)m_deltaTime * 0.5f;
    speed *= 0.9f; // drag

    cameraRotation += {(f32)m_input.mouseYDelta * (f32)m_deltaTime, (f32)m_input.mouseXDelta * (f32)m_deltaTime, 0};
    float aspect = (float)m_platform.windowDimension.width / (float)m_platform.windowDimension.height;

    Mat4 projection = Mat4::GetPerspectiveMatrix(aspect, 90, 0.01f, 100);
    Mat4 view = (Mat4::GetRotationY(-cameraRotation.y) * Mat4::GetRotationX(-cameraRotation.x)).GetTransposedMatrix() *
                Mat4::GetTranslation(cameraPosition);

    static f32 time;
    time += m_deltaTime * 0.1f;
    for (int i = 0; i < (int)m_modelCount; i++) // TODO physics code here ?
    {
        if (m_models[i].parent == nullptr)
        {
            m_models[i].position = Float3(sinf(time * i), cosf(time), 0);
            m_models[i].orientation =  Quaternion::SLerp({ 1,0.2f,0.2f,0.2f }, { 1,0.8f,0.8f,0.8f }, time);
            m_models[i].scale = 1;
        }
    }

    m_graphics.SetViewProjectionMatrix(projection * view);
}

void Engine::Draw()
{
    HDC dc = GetDC(m_platform.m_window);
    m_graphics.Draw(m_models, m_modelCount);
    // swap the buffers to show output

    if (!SwapBuffers(dc))
        m_platform.FatalError("Failed to swap OpenGL buffers!");
    ReleaseDC(m_platform.m_window, dc);

    u64 endTime = RedFoxEngine::Platform::GetTimer();
    m_deltaTime = ((endTime - m_time) / (f64)m_frequency);
}

Engine::~Engine()
{
    for (int i = 0; i < (int)m_modelCount; i++)
        DeInitObj(&m_models[i].obj);
}