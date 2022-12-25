#include "mountains.h"

#ifdef EMSCRIPTEN
#include <new>
#endif

#include <glm/ext/matrix_clip_space.hpp>

#include "easylogging++.h"
#include "platform/opengl/openglresourcemanager.h"

#define KEY_PRESSED_OR_HOLD(key) input.wasKeyPressed(key) || input.isKeyHeld(key)

Mountains::Mountains()
{
    appName = "Mountains";
}

bool Mountains::onUserCreate()
{
    OpenGLResourceManager::loadShader("assets/shaders/shader.vert", "assets/shaders/shader.frag", "shader");
    OpenGLResourceManager::loadShader("assets/shaders/sprite.vert", "assets/shaders/sprite.frag", "sprite");
    OpenGLResourceManager::loadShader("assets/shaders/text.vert", "assets/shaders/text.frag", "text");

    OpenGLResourceManager::loadMesh("assets/models/mountains.obj", "mountains");

    OpenGLResourceManager::loadTexture("assets/images/coffee.png", "coffee");

    OpenGLResourceManager::loadFont("assets/fonts/LeagueGothic-Regular.ttf", "gothic");

    std::shared_ptr<OpenGLShader> shader = OpenGLResourceManager::getShader("shader");
    shader->bind();

    camera = std::make_unique<GameCamera>(80.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 1.f,
                                          18000.0f);

    camera->setPosition(glm::vec3(0.f, 40.f, 70.f));

    glm::mat4 view = camera->getViewMatrix();
    shader->setMat4("view", view);

    glm::mat4 projection = camera->getProjection();
    shader->setMat4("projection", projection);

    shader->setFloat3("lightPos", glm::vec3(40.f, 40.f, 40.f));
    shader->setFloat("ambientStrength", 0.2f);

    sprite = std::make_unique<OpenGLSprite>();

    return true;
}

bool Mountains::onUserUpdate(Uint32 elapsedTime)
{
    if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
        return false;
    }

    if (KEY_PRESSED_OR_HOLD(SDL_SCANCODE_W) || KEY_PRESSED_OR_HOLD(SDL_SCANCODE_UP)) {
        camera->moveForward(elapsedTime);
    } else if (KEY_PRESSED_OR_HOLD(SDL_SCANCODE_S) || KEY_PRESSED_OR_HOLD(SDL_SCANCODE_DOWN)) {
        camera->moveBackward(elapsedTime);
    } else if (KEY_PRESSED_OR_HOLD(SDL_SCANCODE_A) || KEY_PRESSED_OR_HOLD(SDL_SCANCODE_LEFT)) {
        camera->strafeLeft(elapsedTime);
    } else if (KEY_PRESSED_OR_HOLD(SDL_SCANCODE_D) || KEY_PRESSED_OR_HOLD(SDL_SCANCODE_RIGHT)) {
        camera->strafeRight(elapsedTime);
    }

    if (input.isButtonPressed()) {
        camera->mouseMove(input.getMoveDelta());
    }

    camera->mouseScroll(input.getScrollDelta());

    std::shared_ptr<OpenGLShader> shader = OpenGLResourceManager::getShader("shader");
    shader->bind();
    shader->setMat4("view", camera->getViewMatrix());
    shader->setMat4("projection", camera->getProjection());
    shader->setFloat3("viewPos", camera->getPosition());

    auto model = glm::mat4(1.f);
    shader->setMat4("model", model);

    OpenGLResourceManager::getMesh("mountains")->render();

    sprite->render("coffee", glm::vec2(10.f, 10.f), glm::vec2(64.f, 64.f));

    OpenGLResourceManager::getFont("gothic")->renderText(appName, 25.0, 25.0, glm::vec3(0.5, 0.9f, 1.0f));

    return true;
}

bool Mountains::onUserResize(int width, int height)
{
    camera->setViewportSize(width, height);

    glm::mat projection = camera->getProjection();
    OpenGLResourceManager::getShader("shader")->bind()->setMat4("projection", projection);

    projection = glm::ortho(0.f, static_cast<float>(width), static_cast<float>(height), 0.f, -1.f, 1.f);
    OpenGLResourceManager::getShader("sprite")->bind()->setMat4("projection", projection);

    projection = glm::ortho(0.f, static_cast<float>(width), 0.f, static_cast<float>(height));
    OpenGLResourceManager::getShader("text")->bind()->setMat4("projection", projection);

    return true;
}

bool Mountains::onUserDestroy()
{
    return true;
}
