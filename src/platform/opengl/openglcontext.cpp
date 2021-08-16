#include "platform/opengl/openglcontext.h"

#include <SDL.h>
#ifdef EMSCRIPTEN
#include <glad/gles2.h>
#else
#include <glad/gl.h>
#endif

#include "easylogging++.h"

#include "globals.h"

OpenGLContext::OpenGLContext(SDL_Window *window) {
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

#ifdef EMSCRIPTEN
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
        LOG(ERROR) << "OpenGL context could not be created: " << SDL_GetError();
        return;
    }

#ifdef EMSCRIPTEN
    gladLoadGLES2((GLADloadfunc) SDL_GL_GetProcAddress);
#else
    gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
#endif

    if (SDL_GL_MakeCurrent(window, context) < 0) {
        LOG(ERROR) << "Could not be set up OpenGL context for rendering: " << SDL_GetError();
        return;
    }

    if (SDL_GL_SetSwapInterval(1) < 0) {
        LOG(ERROR) << "Unable to set vsync: " << SDL_GetError();
        return;
    }

    this->window = window;

    if (window != nullptr) {
        SDL_GetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);
    } else {
        width = globals::SCREEN_WIDTH;
        height = globals::SCREEN_HEIGHT;
    }
}

OpenGLContext::~OpenGLContext() {
    SDL_GLContext context = SDL_GL_GetCurrentContext();
    SDL_GL_DeleteContext(context);
}

void OpenGLContext::flip() {
    if (window == nullptr) {
        return;
    }
    SDL_GL_SwapWindow(window);
}
