#pragma once

#include <string>
#include <memory>
#include <map>

namespace Renderer {
    class ShaderProgram;
}

namespace Resources {
    class ResourceManager {
        public:
            using ShaderProgramSharedPtr = std::shared_ptr<Renderer::ShaderProgram>;

            ResourceManager(const std::string &executablePath);
            ResourceManager(const ResourceManager&) = delete;
            ResourceManager(ResourceManager&&) = delete;
            ~ResourceManager() = default;

            ResourceManager& operator=(const ResourceManager&) = delete;
            ResourceManager& operator=(ResourceManager&&) = delete;

            ShaderProgramSharedPtr loadShaders(
                const std::string &shaderName,
                const std::string &vertexShaderPath,
                const std::string &fragmentShaderPath
            );
            ShaderProgramSharedPtr getShaderProgram(const std::string &shaderName);

            void loadTexture(const std::string &name, const std::string &texturePath);

        private:
            using ShaderProgramMap = std::map<std::string, ShaderProgramSharedPtr>;
            
            ShaderProgramMap shaderProgramMap;

            std::string path;

            std::string getFileString(const std::string &relativePath) const;
    };
}