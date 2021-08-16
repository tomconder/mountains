#ifndef INCLUDE_OPENGLBUFFER_H
#define INCLUDE_OPENGLBUFFER_H

#include <SDL.h>

#include "renderer/buffer.h"

class OpenGLBuffer : public Buffer {
public:
    explicit OpenGLBuffer(uint32_t size);
    OpenGLBuffer(const float *vertices, uint32_t size);
    ~OpenGLBuffer() override;

    void bind() const override;
    void setData(const unsigned char *data, uint32_t size) override;

private:
    uint32_t id = 0;
};

#endif //INCLUDE_OPENGLBUFFER_H
