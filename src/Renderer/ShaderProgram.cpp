#include "ShaderProgarm.h"

#include <iostream>

namespace Renderer {
    ShaderProgram::ShaderProgram(const std::string &vertexShader, const std::string &fragmentShader) {
        GLuint vertexShaderId = 0;
        if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderId))
        {
            std::cerr << "VERTEX SHADER compile-time error" << std::endl;
            return;
        }

        GLuint fragmentShaderId = 0;
        if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderId))
        {
            glDeleteShader(vertexShaderId);
            std::cerr << "FRAGMENT SHADER compile-time error" << std::endl;
            return;
        }

        if (createProgram(vertexShaderId, fragmentShaderId)) {
            compiled = true;
        }

        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
    }

    bool ShaderProgram::createShader(
        const std::string &source,
        const GLenum shaderType,
        GLuint &shaderId
    ) const {
        shaderId = glCreateShader(shaderType);
        const char *code = source.c_str();
        glShaderSource(shaderId, 1, &code, nullptr);
        glCompileShader(shaderId);

        GLint vertexShaderStatus = 0;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &vertexShaderStatus);
        if (vertexShaderStatus == GL_FALSE) {
            GLint logSize = 0;
            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);

            GLchar errorLog[logSize];
            glGetShaderInfoLog(shaderId, logSize, nullptr, errorLog);

            std::cerr << "ERROR::SHADER: Compile-time error:\n" << errorLog << std::endl;

            return false;
        }

        return true;
    }

    ShaderProgram::ShaderProgram(ShaderProgram &&other) noexcept {
        id = other.id;
        compiled = other.compiled;

        other.id = 0;
        other.compiled = false;
    }

    bool ShaderProgram::createProgram(GLuint vertexShaderId, GLuint fragmentShaderId) {
        id = glCreateProgram();
        glAttachShader(id, vertexShaderId);
        glAttachShader(id, fragmentShaderId);
        glLinkProgram(id);

        GLint programStatus = 0;
        glGetProgramiv(id, GL_LINK_STATUS, &programStatus);
        if (programStatus == GL_FALSE) {
            GLint logSize = 0;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logSize);

            char errorMessage[logSize];
            glGetProgramInfoLog(id, logSize, &logSize, errorMessage);

            std::cerr << "ERROR::SHADER: Link-time error:\n" << errorMessage << std::endl;

            return false;
        }

        return true;
    }

    ShaderProgram::~ShaderProgram() {
        glDeleteProgram(id);
    }

    void ShaderProgram::use() const {
        glUseProgram(id);
    }

    ShaderProgram& ShaderProgram::operator=(ShaderProgram &&other) noexcept {
        glDeleteProgram(id);
        id = other.id;
        compiled = other.compiled;

        other.id = 0;
        other.compiled = false;

        return *this;
    }
}