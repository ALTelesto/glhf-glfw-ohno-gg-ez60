#ifndef LIGHT_3D
#define LIGHT_3D

class Light3D {
    protected:
        glm::vec3 position;
        glm::vec3 color;
        float intensity;

    public:
        Light3D(glm::vec3 position, glm::vec3 color, float intensity);

        virtual void draw(GLuint& shaderProgram);

        void setPosition(glm::vec3 position);
        void setColor(glm::vec3 color);
        void setIntensity(float intensity);

        float getIntensity();
};

class PointLight3D : public Light3D{
    public:
        PointLight3D(glm::vec3 position, glm::vec3 color, float intensity);

        void draw(GLuint& shaderProgram);
};

#endif