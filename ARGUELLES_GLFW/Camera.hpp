#ifndef CAMERA_3D
#define CAMERA_3D

namespace camera {
    class Camera3D {
    protected:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 center;
        glm::mat4 projection;
        float fov;

    public:
        Camera3D(glm::vec3 position, glm::vec3 rotation, glm::vec3 center, float fov) {
            this->position = position;
            this->rotation = rotation;
            this->center = center;
            this->fov = fov;
            this->projection = glm::mat4(1.f);
        }

        virtual void draw(GLuint& shaderProgram) {
            glUseProgram(shaderProgram);

            glm::vec3 worldUp = glm::normalize(glm::vec3(0, 1.0f, 0));

            glm::mat4 viewMatrix = glm::lookAt(position, center, worldUp);
            unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

            unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        }

        void setPosition(glm::vec3 position) {
            this->position = position;
        }
        void setRotation(glm::vec3 rotation) {
            this->rotation = rotation;
        }

        glm::vec3 getPosition() {
            return this->position;
        }
        glm::vec3 getRotation() {
            return this->rotation;
        }
        glm::mat4 getProjection() {
            return this->projection;
        }
    };

    class OrthoCamera : public Camera3D {
    public:
        OrthoCamera(glm::vec3 position, glm::vec3 rotation, glm::vec3 center, float fov) : Camera3D(position, rotation, center, fov) {
            projection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, -1.f, 100.0f);
        }

        void draw(GLuint& shaderProgram) {
            glUseProgram(shaderProgram);

            glm::vec3 worldUp = glm::normalize(glm::vec3(0, 1.0f, 0));

            glm::mat4 viewMatrix = glm::lookAt(position, center, worldUp);
            //viewMatrix = glm::rotate(viewMatrix, glm::radians(-180.f), glm::vec3(0, 1, 0));

            unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

            unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        }
    };

    class PerspectiveCamera : public Camera3D {
    public:
        PerspectiveCamera(glm::vec3 position, glm::vec3 rotation, glm::vec3 center, float fov) : Camera3D(position, rotation, center, fov) {
            projection = glm::perspective(glm::radians(fov), 600.0f / 600.0f, 0.1f, 100.0f);
        }
    };
}

#endif