#include "OpenGLGraphics.hpp"
#include "ObjParser.hpp"
#include "glcorearb.h"
#include "imgui.h"

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
    //	TODO:resize in case of window resize
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // set to FALSE to disable vsync
    wglSwapIntervalEXT(1);
}

// TODO(V. Caraulan): replace with modulable functions from model
void Graphics::InitModel(Model *model)
{
    // vertex buffer containing triangle vertices
    unsigned int vbo;
    {
        glCreateBuffers(1, &vbo);
        glNamedBufferStorage(vbo, model->obj.vertexCount * sizeof(ObjVertex), model->obj.vertices,
                             GL_DYNAMIC_STORAGE_BIT);
    }
    unsigned int ebo;
    {
        glCreateBuffers(1, &ebo);
        glNamedBufferStorage(ebo, sizeof(u32) * model->obj.indexCount, model->obj.indices, GL_DYNAMIC_STORAGE_BIT);
    }
    // vertex input
    {
        glCreateVertexArrays(1, &model->vao);

        int vbuf_index = 0;
        glVertexArrayVertexBuffer(model->vao, vbuf_index, vbo, 0, sizeof(struct ObjVertex));
        glVertexArrayElementBuffer(model->vao, ebo);

        int a_pos = 0;
        glVertexArrayAttribFormat(model->vao, a_pos, 3, GL_FLOAT, GL_FALSE, offsetof(struct ObjVertex, position));
        glVertexArrayAttribBinding(model->vao, a_pos, vbuf_index);
        glEnableVertexArrayAttrib(model->vao, a_pos);

        int a_normal = 1;
        glVertexArrayAttribFormat(model->vao, a_normal, 3, GL_FLOAT, GL_FALSE, offsetof(struct ObjVertex, normal));
        glVertexArrayAttribBinding(model->vao, a_normal, vbuf_index);
        glEnableVertexArrayAttrib(model->vao, a_normal);

        int a_uv = 2;
        glVertexArrayAttribFormat(model->vao, a_uv, 2, GL_FLOAT, GL_FALSE, offsetof(struct ObjVertex, textureUV));
        glVertexArrayAttribBinding(model->vao, a_uv, vbuf_index);
        glEnableVertexArrayAttrib(model->vao, a_uv);
    }
    InitTexture(&model->obj);
}

void Graphics::InitTexture(void *data,int height, int width, GLuint &texture)
{

    glCreateTextures(GL_TEXTURE_2D, 1, &texture);

    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureStorage2D(texture, 1, GL_RGBA32F, width, height);
    glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void Graphics::InitTexture(ObjModel *model)
{
    GLuint texture[10];

    glCreateTextures(GL_TEXTURE_2D, model->images.count, texture);
    for (int i = 0; i < (int)model->materials.count; i++)
    {
        int temp = model->materials.material[i].diffuseMap.index0;
        model->materials.material[i].diffuseMap.index0 = texture[temp];
    }

    //WaitForSingleObject(model->images.thread, INFINITE); // TODO(V. Caraulan): Wrap in waiting for thread function
    for (int i = 0; i < (int)model->images.count; i++)
    {
        glTextureParameteri(texture[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(texture[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(texture[i], GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(texture[i], GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureStorage2D(texture[i], 1, GL_RGBA8, model->images.data[i].width, model->images.data[i].height);
        glTextureSubImage2D(texture[i], 0, 0, 0, model->images.data[i].width, model->images.data[i].height, GL_RGBA,
                            GL_UNSIGNED_BYTE, model->images.data[i].data);
    }
}

void Graphics::InitShaders() // TODO(V. Caraulan):
{
    // fragment & vertex shaders for drawing triangle
    {
        const char *glslVshader =
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
            "out gl_PerVertex { vec4 gl_Position; };       \n" // required because of
                                                               // ARB_separate_shader_objects
            "layout (location=0)                           \n"
            "uniform mat4 vp;\n"
            "layout (location=1)                           \n"
            "uniform mat4 model;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    gl_Position            = (vp * model) * vec4(aPos, 1);\n"
            "    vs_out.FragPosition    = (model * vec4(aPos, 1)).rgb;\n"
            "    vs_out.Normal          = (transpose(inverse(mat4(model))) * vec4(aNormal, 1)).rgb;\n"
            "    vs_out.TexCoord        = aTexCoord;\n"
            "}\n"
            "\n";

        const char *glslFshader =
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
            "}                                             \n";


        const GLchar* imgui_vertex_shader =
            "#version 450 core                             \n"
            "#line " STR(__LINE__) "                     \n\n"
            "layout (location = 0) in vec2 Position;\n"
            "layout (location = 1) in vec2 UV;\n"
            "layout (location = 2) in vec4 Color;\n"
            "layout (location = 0)                           \n"

            "out gl_PerVertex { vec4 gl_Position; };       \n" // required because of
            "uniform mat4 ProjMtx;\n"
            "out vec2 Frag_UV;\n"
            "out vec4 Frag_Color;\n"
            "void main()\n"
            "{\n"
            "    Frag_UV = UV;\n"
            "    Frag_Color = Color;\n"
            "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
            "}\n";

        const GLchar* imgui_fragment_shader =
            "#version 450 core                             \n"
            "#line " STR(__LINE__) "                     \n\n"
            "in vec2 Frag_UV;\n"
            "in vec4 Frag_Color;\n"
            "uniform sampler2D Texture;\n"
            "layout (location = 0) out vec4 Out_Color;\n"
            "void main()\n"
            "{\n"
            "    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
            "}\n";

        m_vshader = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &glslVshader);
        m_fshader = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &glslFshader);

        m_imguivshader = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &imgui_vertex_shader);
        m_imguifshader = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &imgui_fragment_shader);

        GLint linked;
        glGetProgramiv(m_vshader, GL_LINK_STATUS, &linked);
        if (!linked)
        {
            char message[1024];
            glGetProgramInfoLog(m_vshader, sizeof(message), NULL, message);
            OutputDebugStringA(message);
            Assert(!"Failed to create vertex shader!");
        }

        glGetProgramiv(m_fshader, GL_LINK_STATUS, &linked);
        if (!linked)
        {
            char message[1024];
            glGetProgramInfoLog(m_fshader, sizeof(message), NULL, message);
            OutputDebugStringA(message);
            Assert(!"Failed to create fragment shader!");
        }

        glGetProgramiv(m_imguifshader, GL_LINK_STATUS, &linked);
        if (!linked)
        {
            char message[1024];
            glGetProgramInfoLog(m_imguifshader, sizeof(message), NULL, message);
            OutputDebugStringA(message);
            Assert(!"Failed to create fragment shader!");
        }

        glGetProgramiv(m_imguivshader, GL_LINK_STATUS, &linked);
        if (!linked)
        {
            char message[1024];
            glGetProgramInfoLog(m_imguivshader, sizeof(message), NULL, message);
            OutputDebugStringA(message);
            Assert(!"Failed to create fragment shader!");
        }

        glGenProgramPipelines(1, &m_pipeline);
        glUseProgramStages(m_pipeline, GL_VERTEX_SHADER_BIT, m_vshader);
        glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, m_fshader);

        glGenProgramPipelines(1, &m_imguiPipeline);
        glUseProgramStages(m_imguiPipeline, GL_VERTEX_SHADER_BIT, m_imguivshader);
        glUseProgramStages(m_imguiPipeline, GL_FRAGMENT_SHADER_BIT, m_imguivshader);
    }
}

void Graphics::SetViewProjectionMatrix(RedFoxMaths::Mat4 vp)
{
    m_viewProjection = vp;
}

void Graphics::Draw(Model *model, int modelCount)
{
    // clear screen
    glClearColor(0.392f, 0.584f, 0.929f, 1.f);
    glClearDepth(0); // TODO(V. Caraulan): Is this correct ?
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // activate shaders for next draw call
    glBindProgramPipeline(m_pipeline);
    for (int i = 0; i < modelCount; i++)
    {
        DrawModel(model[i]);
    }
}

void Graphics::DrawIMGUI(WindowDimension windowDimension)
{
    // int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    // int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    // if (fb_width <= 0 || fb_height <= 0)

    ImDrawData *draw_data = ImGui::GetDrawData();

    // // glEnable(GL_BLEND);
    // // glBlendEquation(GL_FUNC_ADD);
    // // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    // // glDisable(GL_CULL_FACE);
    // // glDisable(GL_DEPTH_TEST);
    // // glDisable(GL_STENCIL_TEST);
    // // glEnable(GL_SCISSOR_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // glViewport(0, 0, windowDimension.width, windowDimension.height);
    RedFoxMaths::Mat4 ortho = RedFoxMaths::Mat4::GetOrthographicMatrix(0, 0, windowDimension.width, windowDimension.height, 0.01, 100);

    glBindProgramPipeline(m_imguiPipeline);
    glProgramUniformMatrix4fv(m_imguivshader, 0, 1,GL_TRUE, ortho.AsPtr());
//    glBindTextureUnit(0, );//TODO get imgui font 
}

void Graphics::DrawModel(Model model)
{
    GLint u_matrix = 0;
    glProgramUniformMatrix4fv(m_vshader, u_matrix, 1, GL_TRUE, m_viewProjection.AsPtr());

    u_matrix = 1;
    RedFoxMaths::Mat4 modelMatrix = model.GetWorldMatrix();
    glProgramUniformMatrix4fv(m_vshader, u_matrix, 1, GL_TRUE, modelMatrix.AsPtr());

    // provide vertex input
    glBindVertexArray(model.vao);

    GLint diffuseMap = 0;
    glBindTextureUnit(diffuseMap, 0);

    for (int i = 0; i < (int)model.obj.meshCount; i++)
    {
        if (model.obj.materials.count && model.obj.materials.material[model.obj.meshes[i].materialIndex].hasTexture)
            glBindTextureUnit(diffuseMap,
                              model.obj.materials.material[model.obj.meshes[i].materialIndex].diffuseMap.index0);
        else
            glBindTextureUnit(diffuseMap, 0); // TODO: create a default 'missing' texture
        glDrawElements(GL_TRIANGLES, model.obj.meshes[i].indexCount, GL_UNSIGNED_INT,
                       (void *)(model.obj.meshes[i].indexStart * sizeof(u32)));
    }
}
} // namespace RedFoxEngine
