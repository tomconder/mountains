#ifndef INCLUDE_OPENGLELEMENTBUFFER_H
#define INCLUDE_OPENGLELEMENTBUFFER_H

#include <SDL.h>

#include "renderer/buffer.h"

class OpenGLElementBuffer : public Buffer {
public:
    explicit OpenGLElementBuffer(uint32_t size);
    OpenGLElementBuffer(const unsigned int *indices, uint32_t size);
    ~OpenGLElementBuffer() override;

    void bind() const override;
    void setData(const unsigned char *data, uint32_t size) override;

private:
    uint32_t id = 0;
};

#endif //INCLUDE_OPENGLELEMENTBUFFER_H
