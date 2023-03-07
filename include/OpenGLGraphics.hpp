/* date = February 25th 2023 4:19 am */
#pragma once

#ifdef _WIN32
#include "Win32Platform.hpp"
#endif

#include "Model.hpp"
#include "ObjParser.hpp"
#include "OpenGLFunctions.hpp"
#include <GL/gl.h>

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
    void Draw(Model *model, int modelCount);
    void DrawModel(Model model);
    void InitModel(Model *model);
    void InitTexture(ObjModel *model);
    void InitShaders();
    void InitGraphics();
    void SetViewProjectionMatrix(RedFoxMaths::Mat4 vp);
};
} // namespace RedFoxEngine

extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

#define X(type, name) extern type name;
GL_FUNCTIONS(X)
#undef X
