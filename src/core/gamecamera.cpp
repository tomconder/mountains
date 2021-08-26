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
    view = glm::lookAt(eye, center, up);
}

glm::quat GameCamera::getOrientation() const {
    return {glm::vec3(-pitch, -yaw, 0.0f)};
}

void GameCamera::setViewportSize(int viewportWidth, int viewportHeight) {
    this->width = static_cast<float>(viewportWidth);
    this->height = static_cast<float>(viewportHeight);
    updateProjection();
}

void GameCamera::setEye(const glm::vec3 &position) {
    eye = position;
}

void GameCamera::lookAt() {
    view = glm::lookAt(
        glm::vec3(0., 40.f, 70.f),
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(0.f, 1.f, 0.f)
    );
}
