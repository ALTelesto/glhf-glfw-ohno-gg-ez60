#include "Light.hpp"

Light3D::Light3D(glm::vec3 position, glm::vec3 color, float intensity) {
    this->position = position;
    this->color = color;
    this->intensity = intensity;
}

void Light3D::draw(GLuint& shaderProgram) {
    GLuint lightAddress = glGetUniformLocation(shaderProgram, "dlightPos");
    glUniform3fv(lightAddress, 1, glm::value_ptr(position));
    GLuint lightColorAddress = glGetUniformLocation(shaderProgram, "dlightColor");
    glUniform3fv(lightColorAddress, 1, glm::value_ptr(color));
    GLuint lightIntensityAddress = glGetUniformLocation(shaderProgram, "dIntensity");
    glUniform1f(lightIntensityAddress, intensity);
}

void Light3D::setPosition(glm::vec3 position) {
    this->position = position;
}
void Light3D::setColor(glm::vec3 color) {
    this->color = color;
}
void Light3D::setIntensity(float intensity) {
    this->intensity = intensity;
}

float Light3D::getIntensity() {
    return this->intensity;
}



PointLight3D::PointLight3D(glm::vec3 position, glm::vec3 color, float intensity) : Light3D(position, color, intensity) {
}

void PointLight3D::draw(GLuint& shaderProgram) {
    GLuint lightAddress = glGetUniformLocation(shaderProgram, "plightPos");
    glUniform3fv(lightAddress, 1, glm::value_ptr(position));
    GLuint lightColorAddress = glGetUniformLocation(shaderProgram, "plightColor");
    glUniform3fv(lightColorAddress, 1, glm::value_ptr(color));
    GLuint lightIntensityAddress = glGetUniformLocation(shaderProgram, "pIntensity");
    glUniform1f(lightIntensityAddress, intensity);
}
