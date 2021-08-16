#ifndef INCLUDE_GAMECAMERA_H
#define INCLUDE_GAMECAMERA_H

#include "globals.h"
#include "renderer/camera.h"

class GameCamera : public Camera {
public:
    GameCamera() = default;
    GameCamera(float fov, float aspect, float zNear, float zFar);

    ~GameCamera() override = default;

    void setViewportSize(int viewportWidth, int viewportHeight);

    const glm::mat4 &getProjection() const { return projection; }
    const glm::mat4 &getViewMatrix() const { return view; }

    glm::quat getOrientation() const;
    float getPitch() const { return pitch; }
    float getYaw() const { return yaw; }

private:
    void updateProjection();
    void updateView();

    float fov = 80.0f;
    float aspect = 1.333f;
    float zNear = 1.0f;
    float zFar = 1000.0f;

    float pitch = 0.0f;
    float yaw = 0.0f;

    float width = globals::SCREEN_WIDTH * 1.0f;
    float height = globals::SCREEN_HEIGHT * 1.0f;

    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    glm::vec3 eye = {0.0f, 0.0f, 1.0f};
    glm::vec3 center = {0.0f, 0.0f, 0.0f};
    glm::vec3 up = {0.0f, 1.0f, 0.0f};
};

#endif //INCLUDE_GAMECAMERA_H
