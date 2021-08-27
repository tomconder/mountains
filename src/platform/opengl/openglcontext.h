#ifndef INCLUDE_OPENGL_CONTEXT_H
#define INCLUDE_OPENGL_CONTEXT_H

#include "renderer/graphicscontext.h"
#include "glm/vec4.hpp"

struct SDL_Window;

class OpenGLContext : public GraphicsContext {
public:
    explicit OpenGLContext(SDL_Window *window);
    ~OpenGLContext() override;

    void flip() override;

private:
    SDL_Window *window = nullptr;
    int width;
    int height;
    int syncInterval = 0;

    void setVSync(int interval);
};

#endif //INCLUDE_OPENGL_CONTEXT_H
