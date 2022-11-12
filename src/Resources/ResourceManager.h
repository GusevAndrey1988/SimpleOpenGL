#pragma once

#include <string>
#include <memory>
#include <map>

namespace Renderer {
    class ShaderProgram;
    class Texture2D;
}

namespace Resources {
    class ResourceManager {
        public:
            using ShaderProgramSharedPtr = std::shared_ptr<Renderer::ShaderProgram>;
            using TextureSharedPtr = std::shared_ptr<Renderer::Texture2D>;

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

            TextureSharedPtr loadTexture(const std::string &name, const std::string &texturePath);
            TextureSharedPtr getTexture(const std::string &name);

        private:
            using ShaderProgramMap = std::map<std::string, ShaderProgramSharedPtr>;
            using TextureMap = std::map<std::string, TextureSharedPtr>;
            
            ShaderProgramMap shaderProgramMap;
            TextureMap textureMap;

            std::string path;

            std::string getFileString(const std::string &relativePath) const;
    };
}