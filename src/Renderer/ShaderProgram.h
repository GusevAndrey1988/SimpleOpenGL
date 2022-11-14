#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <string>
#include <memory>

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

            void setInt(const std::string &name, GLint value);
            void setMatrix4(const std::string &name, const glm::mat4 &matrix);

            ShaderProgram& operator=(ShaderProgram &&other) noexcept;
            ShaderProgram& operator=(const ShaderProgram&) = delete;

        private:
            bool compiled = false;
            GLuint id = 0;

            static const int LOG_SIZE = 1024;

            bool createShader(
                const std::string &source,
                const GLenum shaderType,
                GLuint &shaderId
            ) const;

            bool createProgram(GLuint vertexShaderId, GLuint fragmentShaderId);
    };

    using ShaderProgramSharedPtr = std::shared_ptr<ShaderProgram>;
}