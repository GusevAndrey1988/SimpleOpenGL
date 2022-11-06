#pragma once

#include <glad/glad.h>
#include <string>

namespace Renderer {
    class ShaderProgram {
        public:
            ShaderProgram(const std::string &vertexShader, const std::string &fragmentShader);
            ShaderProgram(ShaderProgram &&other) noexcept;
            ShaderProgram() = delete;
            ShaderProgram(ShaderProgram&) = delete;

            ~ShaderProgram();

            bool isCompiled() const { return compiled; }
            void use() const;

            ShaderProgram& operator=(ShaderProgram &&other) noexcept;
            ShaderProgram& operator=(const ShaderProgram&) = delete;

        private:
            bool compiled = false;
            GLuint id = 0;

            bool createShader(
                const std::string &source,
                const GLenum shaderType,
                GLuint &shaderId
            ) const;

            bool createProgram(GLuint vertexShaderId, GLuint fragmentShaderId);
    };
}