#include "mountains.h"

#ifdef EMSCRIPTEN
#include <new>
#endif

#include <glm/ext/matrix_clip_space.hpp>

#include "easylogging++.h"
#include "platform/opengl/openglresourcemanager.h"

Mountains::Mountains() {
    appName = "Mountains";
}

bool Mountains::onUserCreate() {
    OpenGLResourceManager::loadShader("assets/shaders/shader.vert", "assets/shaders/shader.frag", "shader");
    OpenGLResourceManager::loadShader("assets/shaders/sprite.vert", "assets/shaders/sprite.frag", "sprite");
    OpenGLResourceManager::loadShader("assets/shaders/text.vert", "assets/shaders/text.frag", "text");

    OpenGLResourceManager::loadMesh("assets/models/mountains.obj", "mountains");

    OpenGLResourceManager::loadTexture("assets/images/coffee.png", "coffee");

    OpenGLResourceManager::loadFont("assets/fonts/LeagueGothic-Regular.ttf", "gothic");

    std::shared_ptr<OpenGLShader> shader = OpenGLResourceManager::getShader("shader");
    shader->bind();

    camera = std::make_unique<GameCamera>(80.0f,
                                          globals::SCREEN_WIDTH * 1.f / globals::SCREEN_HEIGHT,
                                          1.f,
                                          18000.0f);

    camera->setPosition(glm::vec3(0., 40.f, 70.f));

    glm::mat4 view = camera->getViewMatrix();
    shader->setMat4("view", view);

    glm::mat4 proj = camera->getProjection();
    shader->setMat4("proj", proj);

    sprite = std::make_unique<OpenGLSprite>();

    startTime = SDL_GetTicks();

    return true;
}

bool Mountains::onUserUpdate(Uint32 elapsedTime) {
    UNUSED(elapsedTime);

    if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
        return false;
    }

    auto time = SDL_GetTicks() - startTime;

    if (input.wasKeyPressed(SDL_SCANCODE_W) || input.isKeyHeld(SDL_SCANCODE_W)) {
        camera->moveForward(time);
    } else if (input.wasKeyPressed(SDL_SCANCODE_S) || input.isKeyHeld(SDL_SCANCODE_S)) {
        camera->moveBackward(time);
    } else if (input.wasKeyPressed(SDL_SCANCODE_A) || input.isKeyHeld(SDL_SCANCODE_A)) {
        camera->strafeLeft(time);
    } else if (input.wasKeyPressed(SDL_SCANCODE_D) || input.isKeyHeld(SDL_SCANCODE_D)) {
        camera->strafeRight(time);
    }

    if (input.isButtonPressed()) {
        camera->mouseMove(input.getMoveDelta());
    }

    camera->mouseScroll(input.getScrollDelta());

    std::shared_ptr<OpenGLShader> shader = OpenGLResourceManager::getShader("shader");
    shader->bind();
    shader->setMat4("view", camera->getViewMatrix());
    shader->setMat4("proj", camera->getProjection());

    auto model = glm::mat4(1.f);
    shader->setMat4("model", model);

    OpenGLResourceManager::getMesh("mountains")->render();

    sprite->render("coffee", glm::vec2(10.f, 10.f), glm::vec2(64.f, 64.f));

    OpenGLResourceManager::getFont("gothic")->
        renderText("Mountains", 25.0, 25.0, glm::vec3(0.5, 0.9f, 1.0f));

    return true;
}

bool Mountains::onUserResize(int width, int height) {
    camera->setViewportSize(width, height);

    glm::mat proj = camera->getProjection();
    OpenGLResourceManager::getShader("shader")->setMat4("proj", proj);

    proj = glm::ortho(0.f, static_cast<float>(width), static_cast<float>(height), 0.f, -1.f, 1.f);
    OpenGLResourceManager::getShader("sprite")->setMat4("proj", proj);

    proj = glm::ortho(0.f, static_cast<float>(width), 0.f, static_cast<float>(height));
    OpenGLResourceManager::getShader("text")->setMat4("proj", proj);

    return true;
}

bool Mountains::onUserDestroy() {
    return true;
}
