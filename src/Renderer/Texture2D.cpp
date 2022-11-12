#include "./Texture2D.h"

namespace Renderer
{
    Texture2D::Texture2D(
        GLuint _width,
        GLuint _height,
        const unsigned char *data,
        unsigned int channels,
        const GLenum &filter,
        const GLenum &wrapMode
    )
        : width(_width), height(_height), mode(0)
    {
        selectMode(channels);

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            mode,
            width,
            height,
            0,
            mode,
            GL_UNSIGNED_BYTE,
            data
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture2D::selectMode(unsigned int channels) {
        switch (channels) {
            case 3:
                mode = GL_RGB;
                break;
            case 4:
                mode = GL_RGBA;
                break;
            default:
                mode = GL_RGB;
        }   
    }

    Texture2D::~Texture2D() {
        glDeleteTextures(1, &id);
    }

    Texture2D::Texture2D(Texture2D &&other) {
        id = other.id;
        other.id = 0;

        width = other.width;
        height = other.height;
        mode = other.mode;
    }

    Texture2D& Texture2D::operator=(Texture2D &&other) {
        glDeleteTextures(1, &id);

        id = other.id;
        other.id = 0;

        width = other.width;
        height = other.height;
        mode = other.mode;

        return *this;
    }

    void Texture2D::bind() const {
        glBindTexture(GL_TEXTURE_2D, id);
    }
}
