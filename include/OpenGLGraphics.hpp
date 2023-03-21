/* date = February 25th 2023 4:19 am */
#pragma once

#ifdef _WIN32
#include "Win32Platform.hpp"
#endif

#include <GL/gl.h>
#include "OpenGLFunctions.hpp"
#include "ObjParser.hpp"
#include "imgui.h"

namespace RedFoxEngine
{

struct Light
{
    vec3 position;
    float cutOff;
    vec3 direction;
    float outerCutOff;


    vec3 ambient;
    float constant;
    vec3 diffuse;
    float linear;
    vec3 specular;
    float quadratic;
};

class Graphics
{
private:
    RedFoxMaths::Mat4 m_viewProjection;
    GLuint m_pipeline;
    GLuint m_vshader, m_fshader;
    GLuint m_gpipeline;
    GLuint m_gvshader, m_gfshader;
    GLuint m_gBuffer;
    GLuint m_gPosition, m_gNormal, m_gAlbedoSpec;
public:
    GLuint m_imguiFramebuffer;
    GLuint m_imguiTexture;
    void Draw(GameObject *gameObjects, int gameObjectCount, Memory *tempArena);
    void DrawModel(Model *model);
    void DrawModelInstances(Model *model, int instanceCount);
    void InitModel(Model *model);
    void InitTexture(ObjModel *model);
    void InitTexture(void *data,int height, int width, GLuint &texture);
    void InitShaders(Memory *tempArena);
    void InitGraphics(Memory *tempArena, WindowDimension dimension);
    void SetViewProjectionMatrix(RedFoxMaths::Mat4 vp);
};
} // namespace RedFoxEngine

extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

#define X(type, name) extern type name;
GL_FUNCTIONS(X)
#undef X
