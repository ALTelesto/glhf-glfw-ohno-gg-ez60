#ifdef MODEL_3D
#define MODEL_3D

class Model3D {
    private:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::vec3 color;

        std::vector<GLfloat> fullVertexData;
        GLuint VAO, VBO, texture;

        float specStr;
        float specPhong;

    public:
        Model3D(std::string pathModel, std::string pathTexture, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 color);

        void draw(GLuint& shaderProgram);

        void setPosition(glm::vec3 position);
        void setRotation(glm::vec3 rotation);
        void setScale(glm::vec3 scale);
        void setColor(glm::vec3 color);
        void setSpec(glm::vec2 spec);

        glm::vec3 getPosition();
        glm::vec3 getRotation();
};

#endif