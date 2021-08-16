#ifndef INCLUDE_RENDERERAPI_H
#define INCLUDE_RENDERERAPI_H

#include "glm/vec4.hpp"

class RendererAPI {
public:
    RendererAPI() = default;
    virtual ~RendererAPI() = default;

    virtual void init() = 0;
    virtual void setViewport(int32_t x, int32_t y, int32_t width, int32_t height) = 0;

    virtual void setClearColor(const glm::vec4 &color) = 0;
    virtual void clear() = 0;
};

#endif //INCLUDE_RENDERERAPI_H
