#include "BatchRenderer.h"
#include <glad/glad.h>

namespace VREN
{
    void BatchRenderer::Begin()
    {
        instanceCount = 0;
    }

    void BatchRenderer::Submit(const Transform &t, const Material &mat)
    {
        if (instanceCount >= MaxInstances)
            return;

        InstanceData &inst = instanceData[instanceCount++];
        inst.model = t.GetMatrix();

        inst.material.r = mat.Color.r / 255.0f;
        inst.material.g = mat.Color.g / 255.0f;
        inst.material.b = mat.Color.b / 255.0f;
        inst.material.a = mat.Color.a / 255.0f;
    }

    void BatchRenderer::End(std::shared_ptr<PerspectiveCamera> cam, std::shared_ptr<Shader> shader)
    {
        if (instanceCount == 0)
            return;

        Flush(cam, shader);
        instanceCount = 0;
    }

    void BatchRenderer::Flush(std::shared_ptr<PerspectiveCamera> cam, std::shared_ptr<Shader> shader)
    {
        shader->Use();
        shader->Mat4(cam->GetProjection(), "uProj");
        shader->Mat4(Matrix4::Invert(cam->GetView()), "uView");

        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, instanceCount * sizeof(InstanceData), instanceData);

        geometryVAO->Bind();
        glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0, instanceCount);
        glBindVertexArray(0);
    }

    void BatchRenderer::Shutdown()
    {
        if (instanceVBO)
            glDeleteBuffers(1, &instanceVBO);
        geometryVAO.reset();
    }

    void CubeBatchRenderer::Init()
    {
        float vertices[] = {
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f,
            -0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f};

        unsigned int indices[] = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            0, 4, 7, 7, 3, 0,
            1, 5, 6, 6, 2, 1,
            0, 1, 5, 5, 4, 0,
            3, 2, 6, 6, 7, 3};

        geometryVAO = std::make_unique<VertexArray>();
        geometryVAO->Bind();

        geometryVAO->GenerateVertexBuffer(vertices, sizeof(vertices));
        geometryVAO->GenerateIndexBuffer(indices, sizeof(indices));
        geometryVAO->GetVertexBuffer()->AddLayout(0, 0, 3);

        indexCount = sizeof(indices) / sizeof(unsigned int);

        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, MaxInstances * sizeof(InstanceData), nullptr, GL_DYNAMIC_DRAW);

        for (int i = 0; i < 4; i++)
        {
            glEnableVertexAttribArray(1 + i);
            glVertexAttribPointer(
                1 + i,
                4,
                GL_FLOAT,
                GL_FALSE,
                sizeof(InstanceData),
                (void *)(offsetof(InstanceData, model) + sizeof(float) * 4 * i));
            glVertexAttribDivisor(1 + i, 1);
        }

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void *)(sizeof(Matrix4)));
        glVertexAttribDivisor(5, 1);

        glBindVertexArray(0);
    }
}
