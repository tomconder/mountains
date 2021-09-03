#include "core/gamecamera.h"

#include <cmath>

#include "easylogging++.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

GameCamera::GameCamera(float fov, float aspect, float zNear, float zFar)
    : fov(fov), aspect(aspect), zNear(zNear), zFar(zFar) {
    updateProjection();

    glm::vec3 front;
    front.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    front.y = std::sin(glm::radians(pitch));
    front.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));

    cameraFront = glm::normalize(front);
    updateView();
}

void GameCamera::updateProjection() {
    aspect = width / height;
    projection = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
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
    cameraPos -= static_cast<float>(delta) * cameraFront * cameraSpeed;
    updateView();
}

void GameCamera::moveForward(unsigned int delta) {
    cameraPos += static_cast<float>(delta) * cameraFront * cameraSpeed;
    updateView();
}

void GameCamera::strafeLeft(unsigned int delta) {
    cameraPos -= glm::normalize(glm::cross(cameraFront, up)) * static_cast<float>(delta) * cameraSpeed;
    updateView();
}

void GameCamera::strafeRight(unsigned int delta) {
    cameraPos += glm::normalize(glm::cross(cameraFront, up)) * static_cast<float>(delta) * cameraSpeed;
    updateView();
}

void GameCamera::mouseMove(const glm::vec2 &offset) {
    yaw += offset.x * cameraSpeed;
    pitch += offset.y * cameraSpeed;

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

void GameCamera::mouseScroll(const glm::vec2 &offset) {
    if (offset.y == 0) {
        return;
    }

    fov -= offset.y * 5;

    if (fov < 30.f) {
        fov = 30.f;
    }
    if (fov > 90.f) {
        fov = 90.f;
    }

    updateProjection();
}
