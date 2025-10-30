#pragma once

#include "Base.h"
#include "Math/Matrix.h"
#include <string>
#include <unordered_map>

namespace VREN
{
    class VREN_API Shader
    {
    public:
        Shader();
        Shader(const std::string &file);
        Shader(const std::string &vertexSource, const std::string &fragmentSource);
        ~Shader();

        void Load(const std::string &file);
        void Load(const std::string &vertexSource, const std::string &fragmentSource);

        void Use();

        // uniforms
        void Int(int i, const char* name);
        void Mat4(const Matrix4 &mat, const char *name);

        inline u32 GetID() const { return id; };

    private:
        u32 id;
        std::unordered_map<std::string, u32> uniforms;

    private:
        void LoadUniforms();
        void Compile(const char *vertSrc, const char *fragSource);
        u32 CompileShader(u32 type, const char *src);
        u32 GetUniLoc(const char* name);
    };
}