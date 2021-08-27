#include "core/gamecamera.h"

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

void GameCamera::moveBackward(float delta) {
    cameraPos -= delta * cameraFront / 10000.0f;
    updateView();
}

void GameCamera::moveForward(float delta) {
    cameraPos += delta * cameraFront / 10000.0f;
    updateView();
}

void GameCamera::strafeLeft(float delta) {
    cameraPos -= glm::normalize(glm::cross(cameraFront, up)) * delta / 10000.0f;
    updateView();
}

void GameCamera::strafeRight(float delta) {
    cameraPos += glm::normalize(glm::cross(cameraFront, up)) * delta / 10000.0f;
    updateView();
}
