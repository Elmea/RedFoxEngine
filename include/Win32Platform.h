/* date = February 22nd 2023 11:17 am */
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stddef.h>
#include <stdint.h>
#include <new>

#include <GL/gl.h>
#include "glcorearb.h"  // download from https://www.khronos.org/registry/OpenGL/api/GL/glcorearb.h
#include "wglext.h"     // download from https://www.khronos.org/registry/OpenGL/api/GL/wglext.h

typedef int8_t       s8;
typedef uint8_t      u8;
typedef int16_t      s16;
typedef uint16_t     u16;
typedef int32_t      s32;
typedef uint32_t     u32;
typedef int64_t      s64;
typedef uint64_t     u64;

typedef float  f32;
typedef double f64;

namespace RedFoxEngine
{
	typedef HWND      Window;
	typedef HINSTANCE ApplicationContext;

	struct WindowDimension
	{
		int width;
		int height;
	};
	struct Input
	{
		int mouseXPosition;
		int mouseYPosition;
		int mouseXDelta;
		int mouseYDelta;

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
		u8 Enter : 1;
		u8 Z : 1;
		u8 X : 1;
		u8 C : 1;
		u8 V : 1;
		u8 B : 1;
		u8 N: 1;
		u8 M : 1;
		u8 Comma : 1;
		u8 Period : 1;
		u8 Slash : 1;

	};

	class Platform
	{

	private:
		ApplicationContext m_applicationContext;

		public:
		WindowDimension	windowDimension;
		Window			 m_window;
		static int		 m_running;

	private:
		Window CreateRedFoxWindow(int width, int height);

	public:
		Platform() = default;
		Platform(int width, int height);
		void			FatalError(const char* message);
		void 		   MessageProcessing(Input *input);
		WindowDimension GetWindowDimension();
		void			GetWglFunctions();
		static u64	  GetTimerFrequency();
		static u64	  GetTimer();
	};
}

#define GL_FUNCTIONS(X) \
X(PFNGLCREATEBUFFERSPROC,            glCreateBuffers            ) \
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
X(PFNGLBINDTEXTUREUNITPROC,          glBindTextureUnit          ) \
X(PFNGLCREATETEXTURESPROC,           glCreateTextures           ) \
X(PFNGLTEXTUREPARAMETERIPROC,        glTextureParameteri        ) \
X(PFNGLTEXTURESTORAGE2DPROC,         glTextureStorage2D         ) \
X(PFNGLTEXTURESUBIMAGE2DPROC,        glTextureSubImage2D        ) \
X(PFNGLDEBUGMESSAGECALLBACKPROC,     glDebugMessageCallback     )

#define STR2(x) #x
#define STR(x) STR2(x)

#define Assert(cond) do { if (!(cond)) __debugbreak(); } while (0)