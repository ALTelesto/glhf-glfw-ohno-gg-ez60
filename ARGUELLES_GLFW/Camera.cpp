#include "Camera.hpp"

Camera3D::Camera3D(glm::vec3 position, glm::vec3 rotation, glm::vec3 center, float fov) {
    this->position = position;
    this->rotation = rotation;
    this->center = center;
    this->fov = fov;
    this->projection = glm::mat4(1.f);
}

void Camera3D::draw(GLuint& shaderProgram) {
    glUseProgram(shaderProgram);

    glm::vec3 worldUp = glm::normalize(glm::vec3(0, 1.0f, 0));

    glm::mat4 viewMatrix = glm::lookAt(position, center, worldUp);
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Camera3D::setPosition(glm::vec3 position) {
    this->position = position;
}
void Camera3D::setRotation(glm::vec3 rotation) {
    this->rotation = rotation;
}

glm::vec3 Camera3D::getPosition() {
    return this->position;
}
glm::vec3 Camera3D::getRotation() {
    return this->rotation;
}



OrthoCamera::OrthoCamera(glm::vec3 position, glm::vec3 rotation, glm::vec3 center, float fov) : Camera3D(position, rotation, center, fov) {
    projection = glm::ortho(glm::radians(fov), 600.0f / 600.0f, 0.1f, 100.0f);
}



PerspectiveCamera::PerspectiveCamera(glm::vec3 position, glm::vec3 rotation, glm::vec3 center, float fov) : Camera3D(position, rotation, center, fov) {
    projection = glm::perspective(glm::radians(fov), 600.0f / 600.0f, 0.1f, 100.0f);
}
