#include "core/engine.h"

#ifdef EMSCRIPTEN
#include <emscripten/emscripten.h>
#endif

#include <SDL.h>
#include <SDL_image.h>

#include "easylogging++.h"

Engine::Engine() : appName("undefined") {
    screenWidth = globals::SCREEN_WIDTH;
    screenHeight = globals::SCREEN_HEIGHT;
}

globals::Retcode Engine::construct(int width, int height) {
    screenWidth = width;
    screenHeight = height;

    if (screenWidth == 0 || screenHeight == 0) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Mountains", "Screen height or width cannot be zero", nullptr);
        LOG(ERROR) << "Screen height or width cannot be zero";
        return globals::Retcode::FAIL;
    }

    return globals::Retcode::OK;
}

globals::Retcode Engine::start() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0) {
        LOG(ERROR) << "Unable to initialize SDL: " << SDL_GetError();
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Mountains", "Unable to initialize SDL", nullptr);
        return globals::Retcode::FAIL;
    }

    if (int flags = IMG_INIT_PNG | IMG_INIT_JPG; IMG_Init(flags) != flags) {
        LOG(ERROR) << "Unable to initialize SDL image: " << IMG_GetError();
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Mountains", "Unable to initialize SDL image", nullptr);
        return globals::Retcode::FAIL;
    }

    SDL_Window *window = SDL_CreateWindow(appName.c_str(),
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          screenWidth,
                                          screenHeight,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Mountains", "Could not create window", nullptr);
        LOG(ERROR) << "Could not create window: " << SDL_GetError();
        return globals::Retcode::FAIL;
    }

    graphics = std::make_unique<OpenGLContext>(window);

    renderer = std::make_unique<OpenGLRendererAPI>();
    renderer->init();
    renderer->setClearColor(glm::vec4{0.36f, 0.36f, 0.36f, 1.0f});

    if (!onUserCreate()) {
        SDL_DestroyWindow(window);
        return globals::Retcode::OK;
    }

    lastUpdateTime = SDL_GetTicks();

    return globals::Retcode::OK;
}

bool Engine::iterateLoop() {
    SDL_Event event;
    Uint32 currentTime;
    Uint32 elapsedTime;

    input.beginFrame();

    bool quit = false;
    if (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            quit = true;
        } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
            maintainAspectRatio(event.window.data1, event.window.data2);
            renderer->setViewport(offsetx, offsety, w, h);
            onUserResize(w, h);
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.repeat == 0) {
                input.keyDown(event);
            }
        } else if (event.type == SDL_KEYUP) {
            input.keyUp(event);
        }
    }

    renderer->clear();

    currentTime = SDL_GetTicks();
    elapsedTime = currentTime - lastUpdateTime;
    lastUpdateTime = currentTime;

    if (!onUserUpdate(elapsedTime)) {
        quit = true;
    }

    graphics->flip();

    if (quit && onUserDestroy()) {
#ifdef EMSCRIPTEN
        emscripten_cancel_main_loop();
#endif
        SDL_Quit();

        return true;
    }

    return false;
}

bool Engine::onUserCreate() {
    return true;
}

bool Engine::onUserUpdate(Uint32 elapsedTime) {
    UNUSED(elapsedTime);
    return true;
}

bool Engine::onUserResize(int width, int height) {
    UNUSED(width);
    UNUSED(height);
    return true;
}

bool Engine::onUserDestroy() {
    return true;
}

void Engine::maintainAspectRatio(int eventW, int eventH) {
    float aspectRatio = 4.f / 3.f;

    auto width = static_cast<float>(eventW);
    auto height = static_cast<float>(eventH);

    if (float newAspectRatio = width / height; newAspectRatio > aspectRatio) {
        w = static_cast<int>(4.f * height / 3.f);
        h = eventH;
    } else {
        w = eventW;
        h = static_cast<int>(3.f * width / 4.f);
    }

    offsetx = (eventW - w) / 2;
    offsety = (eventH - h) / 2;
}
