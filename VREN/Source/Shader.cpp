#include "Shader.h"
#include "Core/Logger.h"
#include <fstream>
#include <sstream>
#include <glad/glad.h>

namespace VREN
{
    Shader::Shader()
    {
        id = 0;
    }

    Shader::~Shader()
    {
        glDeleteProgram(id);
    }

    Shader::Shader(const std::string &filename)
    {
        Load(filename);
    }

    Shader::Shader(const std::string &vertexSource, const std::string &fragmentSource)
    {
        Load(vertexSource, fragmentSource);
    }

    void Shader::Load(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            VREN_LOG(Error, "Failed to open shader file: %s", filename.c_str());
            return;
        }

        std::stringstream ss;
        ss << file.rdbuf();
        std::string src = ss.str();

        std::string vertSrc, fragSrc;
        size_t vertPos = src.find("// VERTEX");
        size_t fragPos = src.find("// FRAGMENT");

        if (vertPos == std::string::npos || fragPos == std::string::npos)
        {
            VREN_LOG(Error, "Shader file missing // VERTEX or // FRAGMENT markers!");
            return;
        }

        vertSrc = src.substr(vertPos + 9, fragPos - (vertPos + 9));
        fragSrc = src.substr(fragPos + 11);

        Compile(vertSrc.c_str(), fragSrc.c_str());
    }

    void Shader::Load(const std::string &vertexSource, const std::string &fragmentSource)
    {
        Compile(vertexSource.c_str(), fragmentSource.c_str());
    }

    void Shader::Use()
    {
        glUseProgram(id);
    }

    void Shader::Int(int i, const char *name)
    {
        glUniform1i(GetUniLoc(name), i);
    }

    // private details
    void Shader::LoadUniforms()
    {
        uniforms.clear();

        GLint count = 0;
        glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);

        char nameBuffer[256];
        for (GLint i = 0; i < count; i++)
        {
            GLsizei length = 0;
            GLint size = 0;
            GLenum type = 0;
            glGetActiveUniform(id, i, sizeof(nameBuffer), &length, &size, &type, nameBuffer);

            std::string name(nameBuffer, length);

            if (name.size() > 3 && name.substr(name.size() - 3) == "[0]")
                name = name.substr(0, name.size() - 3);

            GLint location = glGetUniformLocation(id, name.c_str());
            uniforms[name] = location;
        }
    }

    void Shader::Compile(const char *vertSrc, const char *fragSrc)
    {
        u32 vert = CompileShader(GL_VERTEX_SHADER, vertSrc);
        u32 frag = CompileShader(GL_FRAGMENT_SHADER, fragSrc);

        id = glCreateProgram();
        glAttachShader(id, vert);
        glAttachShader(id, frag);
        glLinkProgram(id);

        int success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(id, 512, nullptr, infoLog);
            VREN_LOG(Error, "Unable to link program: \n %s", infoLog);
        }

        glDeleteShader(vert);
        glDeleteShader(frag);

        LoadUniforms();
    }

    u32 Shader::CompileShader(u32 type, const char *src)
    {
        u32 shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            VREN_LOG(Error, "Unable to compile shader: \n %s", infoLog);
        }

        return shader;
    }

    u32 Shader::GetUniLoc(const char *name)
    {
        return uniforms.find(name) != uniforms.end() ? uniforms[name] : 0;
    }
}