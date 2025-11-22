#include "Mesh.h"
#include "Geometry/Geometry.h"
#include <glad/glad.h>

namespace VREN
{
    Mesh::Mesh() {}

    Mesh::~Mesh() {}

    void Mesh::Init() { DestroyAndInitArray(); }

    void Mesh::Render()
    {
        if (!array)
            DestroyAndInitArray();

        array->Bind();
        array->GetVertexBuffer()->Bind();
        array->GetIndexBuffer()->Draw();
    }

    void Mesh::DestroyAndInitArray()
    {
        BoxGeometry box{1, 2, 1};
        auto &Vertices = box.Vertices;
        auto &Indices = box.Indices;

        if (array)
            array.reset();

        array = std::make_unique<VertexArray>();

        array->GenerateVertexBuffer(Vertices.data(), Vertices.size() * sizeof(Vertex3D));
        array->GenerateIndexBuffer(Indices.data(), Indices.size() * sizeof(u32));

        auto vbo = array->GetVertexBuffer();
        vbo->AddLayout(0, 0, 3); // assumes Vertex3D starts with vec3 position
        vbo->Bind();
    }

} // namespace VREN
