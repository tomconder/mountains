#ifndef INCLUDE_MOUNTAINS_H
#define INCLUDE_MOUNTAINS_H

#include "core/engine.h"
#include "core/gamecamera.h"
#include "platform/opengl/openglsprite.h"

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
    std::unique_ptr<GameCamera> camera;
    std::unique_ptr<OpenGLSprite> sprite;
};

#endif // INCLUDE_MOUNTAINS_H
