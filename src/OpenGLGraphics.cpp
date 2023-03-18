#include "OpenGLGraphics.hpp"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

namespace RedFoxEngine
{
void Graphics::InitGraphics(Memory *tempArena)
{
    InitShaders(tempArena);

    // setup global GL state
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // set to FALSE to disable vsync
    wglSwapIntervalEXT(1);
}

void Graphics::InitModel(Model *model)
{
    // vertex buffer containing triangle vertices
    unsigned int vbo;
    {
        glCreateBuffers(1, &vbo);
        glNamedBufferStorage(vbo, model->obj.vertexCount * sizeof(ObjVertex), model->obj.vertices, 0);
    }
    unsigned int ebo;
    {
        glCreateBuffers(1, &ebo);
        glNamedBufferStorage(ebo, sizeof(u32) * model->obj.indexCount, model->obj.indices, 0);
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
    DeInitGraphicsObj(&model->obj);
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

    WaitForSingleObject(model->images.thread, INFINITE); // TODO(V. Caraulan): Wrap in waiting for thread function
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

void Graphics::InitShaders(Memory *tempArena)
{
    // fragment & vertex shaders for drawing triangle
    {
        MyString vertexShaderSource = OpenAndReadEntireFile("src\\Shaders\\vertex.vert", tempArena);
        MyString fragmentShaderSource = OpenAndReadEntireFile("src\\Shaders\\fragment.frag", tempArena);

        m_vshader = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vertexShaderSource.data);
        m_fshader = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fragmentShaderSource.data);

        GLint linked;
        glGetProgramiv(m_vshader, GL_LINK_STATUS, &linked);
        if (!linked)
        {
            char message[1024];
            glGetProgramInfoLog(m_vshader, sizeof(message), nullptr, message);
            OutputDebugStringA(message);
            Assert(!"Failed to create vertex shader!");
        }

        glGetProgramiv(m_fshader, GL_LINK_STATUS, &linked);
        if (!linked)
        {
            char message[1024];
            glGetProgramInfoLog(m_fshader, sizeof(message), nullptr, message);
            OutputDebugStringA(message);
            Assert(!"Failed to create fragment shader!");
        }


        glGenProgramPipelines(1, &m_pipeline);
        glUseProgramStages(m_pipeline, GL_VERTEX_SHADER_BIT, m_vshader);
        glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, m_fshader);
    }
}

void Graphics::SetViewProjectionMatrix(RedFoxMaths::Mat4 vp)
{
    m_viewProjection = vp;
}

void Graphics::Draw(GameObject *objects, int gameObjectCount)
{
    // clear screen
    glClearColor(0.392f, 0.584f, 0.929f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // activate shaders for next draw call
    glBindProgramPipeline(m_pipeline);

    GLint u_matrix = 0;
    glProgramUniformMatrix4fv(m_vshader, u_matrix, 1, GL_TRUE, m_viewProjection.AsPtr());

    u_matrix = 1;
    for (int i = 0; i < gameObjectCount; i++)
    {
        if (objects[i].model)
        {
            RedFoxMaths::Mat4 modelMatrix = objects[i].GetWorldMatrix();
            glProgramUniformMatrix4fv(m_vshader, u_matrix, 1, GL_TRUE, modelMatrix.AsPtr());
            DrawModel(objects[i].model);
        }
    }
}

void Graphics::DrawModel(Model *model)
{
    // provide vertex input
    glBindVertexArray(model->vao);

    GLint diffuseMap = 0;
    glBindTextureUnit(diffuseMap, 0);

    for (int i = 0; i < (int)model->obj.meshCount; i++)
    {
        if (model->obj.materials.count && model->obj.materials.material[model->obj.meshes[i].materialIndex].hasTexture)
            glBindTextureUnit(diffuseMap,
                              model->obj.materials.material[model->obj.meshes[i].materialIndex].diffuseMap.index0);
        else
            glBindTextureUnit(diffuseMap, 0); // TODO: create a default 'missing' texture
        glDrawElements(GL_TRIANGLES, model->obj.meshes[i].indexCount ,GL_UNSIGNED_INT, (void *)(model->obj.meshes[i].indexStart * sizeof(u32)));
    }
}
} // namespace RedFoxEngine
