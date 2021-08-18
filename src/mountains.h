#ifndef INCLUDE_MOUNTAINS_H
#define INCLUDE_MOUNTAINS_H

#include "core/engine.h"

#include "core/gamecamera.h"
#include "platform/opengl/openglbuffer.h"
#include "platform/opengl/openglshader.h"
#include "platform/opengl/opengltexture.h"
#include "platform/opengl/openglvertexarray.h"

#include <memory>
#include <string>

class Mountains : public Engine {
public:
    Mountains();

    bool onUserCreate() override;
    bool onUserUpdate(Uint32 elapsedTime) override;
    bool onUserDestroy() override;
    bool onUserResize(int width, int height) override;

private:
    uint32_t startTime = 0;

    std::unique_ptr<GameCamera> camera;
    std::unique_ptr<OpenGLBuffer> vbo;
    std::unique_ptr<OpenGLVertexArray> vao;

    void init2D();
    void drawSprite(glm::vec2 position, glm::vec2 size);
};

#endif //INCLUDE_MOUNTAINS_H
