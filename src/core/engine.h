#ifndef INCLUDE_ENGINE_H
#define INCLUDE_ENGINE_H

#include <string>
#include <memory>

#include "globals.h"
#include "core/input.h"
#include "platform/opengl/openglcontext.h"
#include "platform/opengl/openglrendererapi.h"

class Engine {
public:
    Engine();
    virtual ~Engine() = default;

    globals::Retcode construct(int width, int height);
    globals::Retcode start();

    bool iterateLoop();

    virtual bool onUserCreate();
    virtual bool onUserUpdate(Uint32 elapsedTime);
    virtual bool onUserDestroy();
    virtual bool onUserResize(int width, int height);

    Input input;
    Uint32 lastUpdateTime = 0;
    int screenHeight;
    int screenWidth;
    std::string appName = "undefined";
    std::unique_ptr<OpenGLContext> graphics;
    std::unique_ptr<OpenGLRendererAPI> renderer;

    int offsetx = 0;
    int offsety = 0;
    int w = 0;
    int h = 0;

private:
    void adjustAspectRatio(int eventW, int eventH);
};

#endif //INCLUDE_ENGINE_H
