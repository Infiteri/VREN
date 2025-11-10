#pragma once

#include "Base.h"
#include "Buffer/VertexArray.h"
#include <memory>

namespace VREN
{
    class VREN_API Mesh
    {
    public:
        Mesh();
        ~Mesh();

        void Init();
        void Render();

    private:
        std::unique_ptr<VertexArray> array;

    private:
        void DestroyAndInitArray();
    };
}