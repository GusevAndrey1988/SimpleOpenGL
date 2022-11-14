#pragma once

#include <string>
#include <memory>
#include <map>

namespace Renderer {
    class ShaderProgram;
    using ShaderProgramSharedPtr = std::shared_ptr<ShaderProgram>;

    class Texture2D;
    using Texture2DSharedPtr = std::shared_ptr<Texture2D>;
}

namespace Resources {
    class ResourceManager {
        public:
            ResourceManager(const std::string &executablePath);
            ResourceManager(const ResourceManager&) = delete;
            ResourceManager(ResourceManager&&) = delete;
            ~ResourceManager() = default;

            ResourceManager& operator=(const ResourceManager&) = delete;
            ResourceManager& operator=(ResourceManager&&) = delete;

            Renderer::ShaderProgramSharedPtr loadShaders(
                const std::string &shaderName,
                const std::string &vertexShaderPath,
                const std::string &fragmentShaderPath
            );
            Renderer::ShaderProgramSharedPtr getShaderProgram(const std::string &shaderName);

            Renderer::Texture2DSharedPtr loadTexture(const std::string &name, const std::string &texturePath);
            Renderer::Texture2DSharedPtr getTexture(const std::string &name);

        private:
            using ShaderProgramMap = std::map<std::string, Renderer::ShaderProgramSharedPtr>;
            using TextureMap = std::map<std::string, Renderer::Texture2DSharedPtr>;
            
            ShaderProgramMap shaderProgramMap;
            TextureMap textureMap;

            std::string path;

            std::string getFileString(const std::string &relativePath) const;
    };
}