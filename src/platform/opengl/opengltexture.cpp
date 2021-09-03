#include "platform/opengl/opengltexture.h"

OpenGLTexture::OpenGLTexture() {
    glGenTextures(1, &id);
}

void OpenGLTexture::generate(uint32_t textureWidth,
                             uint32_t textureHeight,
                             uint32_t bytesPerPixel,
                             const unsigned char *data) {
    glBindTexture(GL_TEXTURE_2D, id);

    width = textureWidth;
    height = textureHeight;

    format = GL_RGB;
    if (bytesPerPixel == 4) {
        format = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glActiveTexture(GL_TEXTURE0);
}

OpenGLTexture::~OpenGLTexture() {
    glDeleteTextures(1, &id);
}

void OpenGLTexture::bind() const {
    glBindTexture(GL_TEXTURE_2D, id);
}
