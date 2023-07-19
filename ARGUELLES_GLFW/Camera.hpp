#ifndef CAMERA_3D
#define CAMERA_3D

class Camera3D {
    
    protected:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 center;
        glm::mat4 projection;
        float fov;

    public:
        Camera3D(glm::vec3 position, glm::vec3 rotation, glm::vec3 center, float fov);

        void draw(GLuint& shaderProgram);

        void setPosition(glm::vec3 position);
        void setRotation(glm::vec3 rotation);

        glm::vec3 getPosition();
        glm::vec3 getRotation();
};

class OrthoCamera : public Camera3D {
    public:
        OrthoCamera(glm::vec3 position, glm::vec3 rotation, glm::vec3 center, float fov);
};

class PerspectiveCamera : public Camera3D {
    public:
        PerspectiveCamera(glm::vec3 position, glm::vec3 rotation, glm::vec3 center, float fov);
};

#endif