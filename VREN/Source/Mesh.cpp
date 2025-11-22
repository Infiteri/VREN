#include "Mesh.h"
#include "Geometry/Geometry.h"
#include "Renderer.h"
#include <glad/glad.h>

namespace VREN
{
    Mesh::Mesh()
    {
        geometry = nullptr;
        isInit = false;
    }

    Mesh::~Mesh() {}

    void Mesh::Init() { DestroyAndInitArray(); }

    void Mesh::SetGeometry(std::shared_ptr<Geometry> newGeometry)
    {
        geometry = newGeometry;
        isInit = false;
        DestroyAndInitArray();
    }

    void Mesh::Render()
    {
        if (!array)
        {
            if (!isInit)
                DestroyAndInitArray();
        }

        if (!array)
            return;

        material.Use(Renderer::GetObjectShader());
        array->Bind();
        array->GetVertexBuffer()->Bind();
        array->GetIndexBuffer()->Draw();
    }

    void Mesh::DestroyAndInitArray()
    {
        if (isInit)
            return;
        isInit = true;

        if (!geometry)
            return;

        if (array)
            array.reset();

        auto &vertices = geometry->Vertices;
        auto &indices = geometry->Indices;
        array = std::make_unique<VertexArray>();

        array->GenerateVertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex3D));
        array->GenerateIndexBuffer(indices.data(), indices.size() * sizeof(u32));

        auto vbo = array->GetVertexBuffer();
        vbo->AddLayout(0, 0, 3);
        vbo->Bind();
    }

    void Mesh::SetMaterial(const Material &mat) { material = mat; }

} // namespace VREN
