#include "core/gamecamera.h"

#include <cmath>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

GameCamera::GameCamera(float fov, float aspect, float zNear, float zFar) :
    fov(glm::radians(fov)), aspect(aspect), zNear(zNear), zFar(zFar) {
    updateProjection();
    updateView();
}

void GameCamera::updateProjection() {
    aspect = width / height;
    projection = glm::perspective(fov, aspect, zNear, zFar);
}

void GameCamera::updateView() {
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, up);
}

glm::quat GameCamera::getOrientation() const {
    return {glm::vec3(-pitch, -yaw, 0.0f)};
}

void GameCamera::setViewportSize(int viewportWidth, int viewportHeight) {
    this->width = static_cast<float>(viewportWidth);
    this->height = static_cast<float>(viewportHeight);
    updateProjection();
}

void GameCamera::setPosition(const glm::vec3 &position) {
    cameraPos = position;
    updateView();
}

void GameCamera::moveBackward(unsigned int delta) {
    cameraPos -= static_cast<float>(delta) * cameraFront / 18000.0f;
    updateView();
}

void GameCamera::moveForward(unsigned int delta) {
    cameraPos += static_cast<float>(delta) * cameraFront / 18000.0f;
    updateView();
}

void GameCamera::strafeLeft(unsigned int delta) {
    cameraPos -= glm::normalize(glm::cross(cameraFront, up)) * static_cast<float>(delta) / 18000.0f;
    updateView();
}

void GameCamera::strafeRight(unsigned int delta) {
    cameraPos += glm::normalize(glm::cross(cameraFront, up)) * static_cast<float>(delta) / 18000.0f;
    updateView();
}

void GameCamera::mouseMove(const glm::vec2 &offset) {
    yaw += offset.x * 0.1f;
    pitch += offset.y * 0.1f;

    if (pitch > 89.f) {
        pitch = 89.f;
    }

    if (pitch < -89.f) {
        pitch = -89.f;
    }

    glm::vec3 front;
    front.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    front.y = std::sin(glm::radians(pitch));
    front.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));

    cameraFront = glm::normalize(front);
    updateView();
}
