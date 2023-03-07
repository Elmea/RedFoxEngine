/* date = February 25th 2023 4:19 am */
#pragma once

#ifdef _WIN32
#include "Win32Platform.hpp"
#endif

#include "ObjParser.hpp"
#include "Model.hpp"

namespace RedFoxEngine
{

	struct Light {
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
}

extern PFNWGLCHOOSEPIXELFORMATARBPROC    wglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNWGLSWAPINTERVALEXTPROC         wglSwapIntervalEXT;

#define GL_FUNCTIONS(X) \
X(PFNGLCREATEBUFFERSPROC,            glCreateBuffers            ) \
X(PFNGLNAMEDFRAMEBUFFERTEXTUREPROC,  glNamedFramebufferTexture  ) \
X(PFNGLCREATEFRAMEBUFFERSPROC,       glCreateFramebuffers       ) \
X(PFNGLBLITFRAMEBUFFERPROC,					 glBlitFramebuffer          ) \
X(PFNGLBINDFRAMEBUFFERPROC,          glBindFramebuffer          ) \
X(PFNGLFRAMEBUFFERTEXTURE2DPROC,     glFramebufferTexture2D     ) \
X(PFNGLGENRENDERBUFFERSPROC,         glGenRenderbuffers         ) \
X(PFNGLDRAWBUFFERSPROC,              glDrawBuffers              ) \
X(PFNGLBINDRENDERBUFFERPROC,         glBindRenderbuffer         ) \
X(PFNGLRENDERBUFFERSTORAGEPROC,      glRenderbufferStorage      ) \
X(PFNGLFRAMEBUFFERRENDERBUFFERPROC,  glFramebufferRenderbuffer  ) \
X(PFNGLCHECKFRAMEBUFFERSTATUSPROC,   glCheckFramebufferStatus   ) \
X(PFNGLNAMEDBUFFERSTORAGEPROC,       glNamedBufferStorage       ) \
X(PFNGLBINDVERTEXARRAYPROC,          glBindVertexArray          ) \
X(PFNGLCREATEVERTEXARRAYSPROC,       glCreateVertexArrays       ) \
X(PFNGLVERTEXARRAYATTRIBBINDINGPROC, glVertexArrayAttribBinding ) \
X(PFNGLVERTEXARRAYVERTEXBUFFERPROC,  glVertexArrayVertexBuffer  ) \
X(PFNGLVERTEXARRAYATTRIBFORMATPROC,  glVertexArrayAttribFormat  ) \
X(PFNGLVERTEXARRAYELEMENTBUFFERPROC, glVertexArrayElementBuffer ) \
X(PFNGLENABLEVERTEXARRAYATTRIBPROC,  glEnableVertexArrayAttrib  ) \
X(PFNGLCREATESHADERPROGRAMVPROC,     glCreateShaderProgramv     ) \
X(PFNGLGETPROGRAMIVPROC,             glGetProgramiv             ) \
X(PFNGLGETPROGRAMINFOLOGPROC,        glGetProgramInfoLog        ) \
X(PFNGLGENPROGRAMPIPELINESPROC,      glGenProgramPipelines      ) \
X(PFNGLUSEPROGRAMSTAGESPROC,         glUseProgramStages         ) \
X(PFNGLBINDPROGRAMPIPELINEPROC,      glBindProgramPipeline      ) \
X(PFNGLPROGRAMUNIFORMMATRIX2FVPROC,  glProgramUniformMatrix2fv  ) \
X(PFNGLPROGRAMUNIFORMMATRIX3FVPROC,  glProgramUniformMatrix3fv  ) \
X(PFNGLPROGRAMUNIFORMMATRIX3FVPROC,  glProgramUniformMatrix4fv  ) \
X(PFNGLPROGRAMUNIFORM1IPROC,         glProgramUniform1i         ) \
X(PFNGLBINDTEXTUREUNITPROC,          glBindTextureUnit          ) \
X(PFNGLCREATETEXTURESPROC,           glCreateTextures           ) \
X(PFNGLTEXTUREPARAMETERIPROC,        glTextureParameteri        ) \
X(PFNGLTEXTURESTORAGE2DPROC,         glTextureStorage2D         ) \
X(PFNGLTEXTURESUBIMAGE2DPROC,        glTextureSubImage2D        ) \
X(PFNGLDEBUGMESSAGECALLBACKPROC,     glDebugMessageCallback     )

#define X(type, name) extern type name;
GL_FUNCTIONS(X)
#undef X
