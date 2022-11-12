#include "./ResourceManager.h"
#include "../Renderer/ShaderProgarm.h"
#include "../Renderer/Texture2D.h"

#include <sstream>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "./stb_image.h"

namespace Resources {
    ResourceManager::ResourceManager(const std::string &executablePath) {
        std::string::size_type foundPosition = executablePath.find_last_of("/\\");
        path = executablePath.substr(0, foundPosition);
    }

    ResourceManager::ShaderProgramSharedPtr ResourceManager::loadShaders(
        const std::string &shaderName,
        const std::string &vertexShaderPath,
        const std::string &fragmentShaderPath
    ) {
        std::string vertexShaderCode = getFileString(vertexShaderPath);
        if (vertexShaderCode.empty()) {
            std::cerr << "No vertex shader!" << std::endl;
            return nullptr;
        }

        std::string fragmentShaderCode = getFileString(fragmentShaderPath);
        if (fragmentShaderPath.empty()) {
            std::cerr << "No fragment shader!" << std::endl;
            return nullptr;
        }

        auto emplaceResult = shaderProgramMap.emplace(
            shaderName,
            ResourceManager::ShaderProgramSharedPtr(
                new Renderer::ShaderProgram(
                    vertexShaderCode,
                    fragmentShaderCode
                )
            )
        );

        if (emplaceResult.second && emplaceResult.first->second->isCompiled()) {
            return emplaceResult.first->second;
        }

        std::cerr << "Can't load shader program:\n"
            << "Vertex: " << vertexShaderPath << "\n"
            << "Fragment: " << fragmentShaderPath << std::endl;

        return nullptr;
    }

    std::string ResourceManager::getFileString(const std::string &relativePath) const {
        std::ifstream f;
        f.open(path + "/" + relativePath, std::ios::in | std::ios::binary);
        if (!f.is_open()) {
            std::cerr << "Failed to open file: " << relativePath << std::endl;
            return std::string{};
        }

        std::stringstream buffer;
        buffer << f.rdbuf();
        return buffer.str();
    }

    ResourceManager::ShaderProgramSharedPtr ResourceManager::getShaderProgram(
        const std::string &shaderName
    ) {
        auto it = shaderProgramMap.find(shaderName);
        if (it != shaderProgramMap.end()) {
            return it->second;
        }

        std::cerr << "Can't find the shader program: " << shaderName << std::endl;
        return nullptr;
    }

    ResourceManager::TextureSharedPtr ResourceManager::loadTexture(
        const std::string &name,
        const std::string &texturePath
    ) {
        int channels = 0;
        int width = 0;
        int height = 0;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *pixels = stbi_load(
            std::string(path + "/" + texturePath).c_str(),
            &width,
            &height,
            &channels,
            0
        );
        
        if (!pixels) {
            std::cerr << "Can't load image: " << texturePath << std::endl;
            return nullptr;
        }

        auto emplaceResult = textureMap.emplace(
            name, TextureSharedPtr(new Renderer::Texture2D(
                width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE
            ))
        );

        if (!emplaceResult.second) {
            std::cerr << "Can't emplace image" << std::endl;
            return nullptr;
        }

        stbi_image_free(pixels);

        return emplaceResult.first->second;
    }

    ResourceManager::TextureSharedPtr ResourceManager::getTexture(const std::string &name) {
        auto textureIt = textureMap.find(name);
        if (textureIt == textureMap.end()) {
            std::cerr << "Can't find the texture: " << name << std::endl;
            return nullptr;
        }

        return textureIt->second;
    }
}
