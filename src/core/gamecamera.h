#ifndef INCLUDE_GAMECAMERA_H
#define INCLUDE_GAMECAMERA_H

#include <glm/mat4x4.hpp>
#include <glm/ext/quaternion_float.hpp>

#include "globals.h"
#include "renderer/camera.h"

class GameCamera : public Camera {
public:
    GameCamera() = default;
    GameCamera(float fov, float width, float height, float zNear, float zFar);

    ~GameCamera() override = default;

    void setViewportSize(int viewportWidth, int viewportHeight);

    const glm::mat4 &getProjection() const { return projection; }
    const glm::mat4 &getViewMatrix() const { return view; }

    glm::quat getOrientation() const;
    float getPitch() const { return pitch; }
    float getYaw() const { return yaw; }

    void setPosition(const glm::vec3 &position);
    glm::vec3 getPosition() { return cameraPos; }

    void moveForward(unsigned int delta);
    void moveBackward(unsigned int delta);
    void strafeLeft(unsigned int delta);
    void strafeRight(unsigned int delta);

    void mouseMove(const glm::vec2 &offset);
    void mouseScroll(const glm::vec2 &offset);

private:
    void updateProjection();
    void updateView();

    float fov = 45.0f;
    float zNear = 1.0f;
    float zFar = 1000.0f;

    float pitch = 0.0f;
    float yaw = -90.0f;

    float cameraSpeed = 0.1f;

    float width = globals::SCREEN_WIDTH * 1.0f;
    float height = globals::SCREEN_HEIGHT * 1.0f;

    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    glm::vec3 cameraPos = {0.0f, 0.0f, 1.0f};
    glm::vec3 up = {0.0f, 1.0f, 0.0f};
    glm::vec3 cameraFront = {0.0f, 0.0f, -1.0f};
};

#endif //INCLUDE_GAMECAMERA_H
