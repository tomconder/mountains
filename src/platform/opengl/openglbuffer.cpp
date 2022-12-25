#include "platform/opengl/openglbuffer.h"

#ifdef EMSCRIPTEN
#include <glad/gles2.h>
#else
#include <glad/gl.h>
#endif

OpenGLBuffer::OpenGLBuffer(uint32_t size) {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLBuffer::OpenGLBuffer(const float *vertices, uint32_t size) {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
}

OpenGLBuffer::~OpenGLBuffer() {
    glDeleteBuffers(1, &id);
}

void OpenGLBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void OpenGLBuffer::setData(const unsigned char *data, uint32_t size) {
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}
