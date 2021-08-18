#include "mountains.h"

#ifdef EMSCRIPTEN
#include <new>
#endif

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "easylogging++.h"
#include "platform/opengl/openglresourcemanager.h"

Mountains::Mountains() {
    appName = "Mountains";
}

bool Mountains::onUserCreate() {
    static const std::array<float, 80> vertices = {
        -0.5f, 0.5f, 0.5f, 0.f, 1.f,
        -0.5f, -0.5f, 0.5f, 0.f, 0.f,
        0.5f, -0.5f, 0.5f, 1.f, 0.f,
        0.5f, 0.5f, 0.5f, 1.f, 1.f,

        0.5f, 0.5f, -0.5f, 0.f, 1.f,
        0.5f, -0.5f, -0.5f, 0.f, 0.f,
        -0.5f, -0.5f, -0.5f, 1.f, 0.f,
        -0.5f, 0.5f, -0.5f, 1.f, 1.f,

        -0.5f, 0.5f, 0.5f, 0.f, 1.f,
        0.5f, 0.5f, 0.5f, 0.f, 0.f,
        0.5f, 0.5f, -0.5f, 1.f, 0.f,
        -0.5f, 0.5f, -0.5f, 1.f, 1.f,

        -0.5f, -0.5f, 0.5f, 0.f, 1.f,
        0.5f, -0.5f, 0.5f, 0.f, 0.f,
        0.5f, -0.5f, -0.5f, 1.f, 0.f,
        -0.5f, -0.5f, -0.5f, 1.f, 1.f
    };

    static const std::array<GLuint, 36> indices = {
        0, 1, 2,
        0, 2, 3,

        7, 6, 1,
        7, 1, 0,

        4, 5, 6,
        4, 6, 7,

        3, 2, 5,
        3, 5, 4,

        8, 9, 10,
        8, 10, 11,

        12, 14, 13,
        12, 15, 14
    };

    OpenGLResourceManager::loadShader("assets/shaders/shader.vert", "assets/shaders/shader.frag", "shader");
    OpenGLResourceManager::loadShader("assets/shaders/sprite.vert", "assets/shaders/sprite.frag", "sprite");
    OpenGLResourceManager::loadShader("assets/shaders/text.vert", "assets/shaders/text.frag", "text");

    OpenGLResourceManager::loadMesh("assets/models/mountains.obj", "mountains");

    OpenGLResourceManager::loadTexture("assets/images/crate.jpg", "crate");
    OpenGLResourceManager::loadTexture("assets/images/coffee.png", "coffee");

    OpenGLResourceManager::loadFont("assets/fonts/LeagueGothic-Regular.ttf", "gothic");

    vertexArray[0] = std::make_unique<OpenGLVertexArray>();
    vertexArray[0]->bind();

    buffer[0] = std::make_unique<OpenGLBuffer>(vertices.data(), sizeof(vertices));
    buffer[0]->bind();

    GLuint elements;
    glGenBuffers(1, &elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

    std::shared_ptr<OpenGLShader> shader = OpenGLResourceManager::getShader("shader");
    shader->bind();

    GLuint program = shader->getId();

    auto position = static_cast<GLuint>(glGetAttribLocation(program, "position"));
    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)nullptr);
    glEnableVertexAttribArray(position);

    position = static_cast<GLuint>(glGetAttribLocation(program, "texCoord"));
    glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(position);

    shader->setInteger("ourTexture1", 0);

    camera = std::make_unique<GameCamera>(80.0f,
                                          globals::SCREEN_WIDTH * 1.f / globals::SCREEN_HEIGHT,
                                          1.f,
                                          100.0f);

    glm::mat4 view = camera->getViewMatrix();
    shader->setMat4("view", view);

    glm::mat4 proj = camera->getProjection();
    shader->setMat4("proj", proj);

    init2D();

    startTime = SDL_GetTicks();

    return true;
}

bool Mountains::onUserUpdate(Uint32 elapsedTime) {
    UNUSED(elapsedTime);

    if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
        return false;
    }

    auto time = SDL_GetTicks() - startTime;

    const float radius = 7.0f;
    float camX = std::sin(static_cast<float>(time) / 1000.0f) * radius;
    float camZ = std::cos(static_cast<float>(time) / 1000.0f) * radius;

    glm::mat4 view = glm::lookAt(
        glm::vec3(camX, 0.f, camZ),
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(0.f, 1.f, 0.f)
    );

    std::shared_ptr<OpenGLShader> shader = OpenGLResourceManager::getShader("shader");
    shader->bind();
    shader->setMat4("view", view);

    OpenGLResourceManager::getTexture("crate")->bind();
    vertexArray[0]->bind();

    auto model = glm::mat4(1.f);
    float angle = 20.0f;
    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.f, 0.3f, 0.5f));
    shader->setMat4("model", model);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

    OpenGLResourceManager::getMesh("mountains")->render();

    drawSprite(glm::vec2(10.f, 10.f), glm::vec2(64.f, 64.f));

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

void Mountains::init2D() {
    std::array<float, 24> vertices = {
        // pos      // tex
        0.f, 1.f, 0.f, 1.f,
        1.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 0.f,

        0.f, 1.f, 0.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        1.f, 0.f, 1.f, 0.f
    };

    OpenGLResourceManager::getShader("sprite")->bind();

    glm::mat4 proj = glm::ortho(0.f, globals::SCREEN_WIDTH * 1.f, globals::SCREEN_HEIGHT * 1.f, 0.f, -1.f, 1.f);
    OpenGLResourceManager::getShader("sprite")->setMat4("proj", proj);

    vertexArray[1] = std::make_unique<OpenGLVertexArray>();
    vertexArray[1]->bind();

    buffer[1] = std::make_unique<OpenGLBuffer>(vertices.data(), sizeof(vertices));
    buffer[1]->bind();

    GLuint program = OpenGLResourceManager::getShader("sprite")->getId();
    auto position = static_cast<GLuint>(glGetAttribLocation(program, "vertex"));
    glVertexAttribPointer(position, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *)nullptr);
    glEnableVertexAttribArray(position);
}

void Mountains::drawSprite(glm::vec2 position, glm::vec2 size) {
    glDisable(GL_DEPTH_TEST);

    auto model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(position, 0.f));

    model = glm::scale(model, glm::vec3(size, 1.f));

    std::shared_ptr<OpenGLShader> shader = OpenGLResourceManager::getShader("sprite");
    shader->bind();
    shader->setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    OpenGLResourceManager::getTexture("coffee")->bind();

    vertexArray[1]->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glEnable(GL_DEPTH_TEST);
}
