#include "OpenGLGraphics.h"

namespace RedFoxEngine
{
	void Graphics::InitGraphics()
	{
		InitShaders();

		// setup global GL state
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_GREATER); // TODO(V. Caraulan): if I'm misusing the matrices, we have to remove this
			glEnable(GL_CULL_FACE);
		}
		// set to FALSE to disable vsync
		wglSwapIntervalEXT(1);
	}

	void Graphics::Draw(ObjModel *model, WindowDimension windowDimension)
	{
		glViewport(0, 0, windowDimension.width, windowDimension.height);

		// clear screen
		glClearColor(0.392f, 0.584f, 0.929f, 1.f);
		glClearDepth(0); // TODO(V. Caraulan): Is this correct ?
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// activate shaders for next draw call
		glBindProgramPipeline(pipeline);

		// TODO(V. Caraulan): loop for all models
		DrawModel(model);
	}

	void Graphics::DrawModel(ObjModel *model)
	{

		// provide vertex input
		glBindVertexArray(vao);

		GLint diffuseMap = 0;
		glBindTextureUnit(diffuseMap, 0);

		for (int i = 0; i < model->meshCount; i++)
		{
			if (model->materials.count && model->materials.material[model->meshes[i].materialIndex].hasTexture)
				glBindTextureUnit(diffuseMap, model->materials.material[model->meshes[i].materialIndex].diffuseMap.index0);
			//			else
			//				glBindTextureUnit(diffuseMap, 0);
			glDrawElements(GL_TRIANGLES, model->meshes[i].indexCount, GL_UNSIGNED_INT, (void*)(model->meshes[i].indexStart * sizeof(u32)));
		}
	}

	// TODO(V. Caraulan): replace with modulable functions from model
	void Graphics::InitModel(ObjModel *model)
	{
		// vertex buffer containing triangle vertices
		unsigned int vbo;
		{
			glCreateBuffers(1, &vbo);
			glNamedBufferStorage(vbo, model->vertexCount * sizeof(ObjVertex), model->vertices, GL_DYNAMIC_STORAGE_BIT);
		}
		unsigned int ebo;
		{
			glCreateBuffers(1, &ebo);
			glNamedBufferStorage(ebo, sizeof(u32)  * model->IndexCount, model->indices, GL_DYNAMIC_STORAGE_BIT);
		}
		// vertex input
		{
			glCreateVertexArrays(1, &vao);


			int vbuf_index = 0;
			glVertexArrayVertexBuffer(vao, vbuf_index, vbo, 0, sizeof(struct ObjVertex));
			glVertexArrayElementBuffer(vao, ebo);

			int a_pos = 0;
			glVertexArrayAttribFormat(vao, a_pos, 3, GL_FLOAT, GL_FALSE, offsetof(struct ObjVertex, position));
			glVertexArrayAttribBinding(vao, a_pos, vbuf_index);
			glEnableVertexArrayAttrib(vao, a_pos);

			int a_normal = 1;
			glVertexArrayAttribFormat(vao, a_normal, 3, GL_FLOAT, GL_FALSE, offsetof(struct ObjVertex, normal));
			glVertexArrayAttribBinding(vao, a_normal, vbuf_index);
			glEnableVertexArrayAttrib(vao, a_normal);

			int a_uv = 2;
			glVertexArrayAttribFormat(vao, a_uv, 2, GL_FLOAT, GL_FALSE, offsetof(struct ObjVertex, textureUV));
			glVertexArrayAttribBinding(vao, a_uv, vbuf_index);
			glEnableVertexArrayAttrib(vao, a_uv);

		}
		InitTexture(model);
	}

	void Graphics::InitTexture(ObjModel *model)
	{
		GLuint texture[10];

		glCreateTextures(GL_TEXTURE_2D, model->images.count, texture);
		for (int i = 0; i < model->materials.count; i++)
		{
			int temp = model->materials.material[i].diffuseMap.index0;
			model->materials.material[i].diffuseMap.index0 = texture[temp];
		}

		WaitForSingleObject(model->images.thread, INFINITE); // TODO(V. Caraulan): Wrap in waiting for thread function
		for (int i = 0; i < model->images.count; i++)
		{
			glTextureParameteri(texture[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureParameteri(texture[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTextureParameteri(texture[i], GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(texture[i], GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureStorage2D(texture[i], 1, GL_RGBA8, model->images.data[i].width, model->images.data[i].height);
			glTextureSubImage2D(texture[i], 0, 0, 0, model->images.data[i].width, model->images.data[i].height, GL_RGBA, GL_UNSIGNED_BYTE, model->images.data[i].data);
		}
	}

	void Graphics::InitShaders()// TODO(V. Caraulan):
	{
		// fragment & vertex shaders for drawing triangle
		{
			const char *glsl_vshader =
				"#version 450 core\n"
				"#line " STR(__LINE__) "                     \n\n"
				"layout(location = 0) in vec3 aPos;\n"
				"layout(location = 1) in vec3 aNormal;\n"
				"layout(location = 2) in vec2 aTexCoord;\n"
				"\n"
				"out VS_OUT\n"
				"{\n"
				"    vec3 FragPosition;\n"
				"    vec3 Normal;\n"
				"    vec2 TexCoord;\n"
				"} vs_out;\n"
				"\n"
				"out gl_PerVertex { vec4 gl_Position; };       \n" // required because of ARB_separate_shader_objects
				"layout (location=0)                           \n"
				"uniform mat4 vp;\n"
				"layout (location=1)                           \n"
				"uniform mat4 model;\n"
				"layout (location=2)                           \n"
				"uniform mat4 normal;\n"
				"\n"
				"void main()\n"
				"{\n"
				"    gl_Position            = (vp * model) * vec4(aPos, 1);\n"
				"	vs_out.FragPosition    = (model * vec4(aPos, 1)).rgb;\n"
				"    vs_out.Normal          = (transpose(inverse(mat4(model))) * vec4(aNormal, 1)).rgb;\n"
				"    vs_out.TexCoord        = aTexCoord;\n"
				"}\n"
				"\n";

			const char* glsl_fshader =
				"#version 450 core                             \n"
				"#line " STR(__LINE__) "                     \n\n" // actual line number in this file for nicer error messages
				"                                              \n"
				"in VS_IN                                      \n"
				"{                                             \n"
				"    vec3 FragPosition;                        \n"
				"    vec3 Normal;                              \n"
				"    vec2 TexCoord;                            \n"
				"} vs_in;                                      \n"
				"                                              \n"
				"layout (binding=0)                            \n" // (from ARB_shading_language_420pack)
				"uniform sampler2D diffuseMap;                 \n" // texture unit binding 0
				"                                              \n"
				"layout (location=0)                           \n"
				"out vec4 o_color;                             \n" // output fragment data location 0
				"                                              \n"
				"void main()                                   \n"
				"{                                             \n"
				"    o_color = texture(diffuseMap, vs_in.TexCoord); \n"
				"}                                             \n"
				;

			vshader = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &glsl_vshader);
			fshader = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &glsl_fshader);

			GLint linked;
			glGetProgramiv(vshader, GL_LINK_STATUS, &linked);
			if (!linked)
			{
				char message[1024];
				glGetProgramInfoLog(vshader, sizeof(message), NULL, message);
				OutputDebugStringA(message);
				Assert(!"Failed to create vertex shader!");
			}

			glGetProgramiv(fshader, GL_LINK_STATUS, &linked);
			if (!linked)
			{
				char message[1024];
				glGetProgramInfoLog(fshader, sizeof(message), NULL, message);
				OutputDebugStringA(message);
				Assert(!"Failed to create fragment shader!");
			}

			glGenProgramPipelines(1, &pipeline);
			glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vshader);
			glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fshader);
		}
	}
}
