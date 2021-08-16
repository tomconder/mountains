#ifndef INCLUDE_OPENGLRENDERERAPI_H
#define INCLUDE_OPENGLRENDERERAPI_H

#include "renderer/rendererapi.h"
#include "glm/vec4.hpp"

class OpenGLRendererAPI : public RendererAPI {
public:
    void init() override;
    void setViewport(int32_t x, int32_t y, int32_t width, int32_t height) override;

    void setClearColor(const glm::vec4 &color) override;
    void clear() override;
};

#endif //INCLUDE_OPENGLRENDERERAPI_H
