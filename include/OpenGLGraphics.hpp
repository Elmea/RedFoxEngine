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
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

class Graphics
{
private:
    RedFoxMaths::Mat4 m_viewProjection;
    GLuint m_pipeline;
    GLuint m_vshader, m_fshader;
public:
    void Draw(GameObject *gameObjects, int gameObjectCount);
    void DrawModel(Model *model);
    void InitModel(Model *model, Memory *temp);
    void InitTexture(ObjModel *model);
    void InitTexture(void *data,int height, int width, GLuint &texture);
    void InitShaders(Memory *tempArena);
    void InitGraphics(Memory *tempArena);
    void SetViewProjectionMatrix(RedFoxMaths::Mat4 vp);
};
} // namespace RedFoxEngine

extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

#define X(type, name) extern type name;
GL_FUNCTIONS(X)
#undef X
