#ifndef LIGHT_3D
#define LIGHT_3D

namespace light {
    class Light3D {
    protected:
        glm::vec3 position;
        glm::vec3 color;
        float intensity;

    public:
        Light3D(glm::vec3 position, glm::vec3 color, float intensity) {
            this->position = position;
            this->color = color;
            this->intensity = intensity;
        }

        virtual void draw(GLuint& shaderProgram) = 0;

        void setPosition(glm::vec3 position) {
            this->position = position;
        }
        void setColor(glm::vec3 color) {
            this->color = color;
            std::cout << color.x << color.y << color.z << std::endl;
        }
        void setIntensity(float intensity) {
            this->intensity = intensity;
        }

        float getIntensity() {
            return this->intensity;
        }
    };

    class DirectionalLight : public Light3D {
    public:
        DirectionalLight(glm::vec3 position, glm::vec3 color, float intensity) : Light3D(position, color, intensity) {
        }

        void draw(GLuint& shaderProgram) {
            GLuint lightAddress = glGetUniformLocation(shaderProgram, "dlightPos");
            glUniform3fv(lightAddress, 1, glm::value_ptr(position));
            GLuint lightColorAddress = glGetUniformLocation(shaderProgram, "dlightColor");
            glUniform3fv(lightColorAddress, 1, glm::value_ptr(color));
            GLuint lightIntensityAddress = glGetUniformLocation(shaderProgram, "dIntensity");
            glUniform1f(lightIntensityAddress, intensity);
        }
    };

    class PointLight3D : public Light3D {
    private:
        glm::vec3 rotation;

    public:
        PointLight3D(glm::vec3 position, glm::vec3 color, float intensity) : Light3D(position, color, intensity) {
            this->rotation = glm::vec3(0.f, 0.f, 0.f);
        }

        void draw(GLuint& shaderProgram) {
            glm::mat4 transformation_matrix = glm::translate(glm::mat4(1.0f), this->position);

            transformation_matrix = glm::rotate(transformation_matrix, glm::radians(this->rotation.x), glm::vec3(1, 0, 0));
            transformation_matrix = glm::rotate(transformation_matrix, glm::radians(this->rotation.y), glm::vec3(0, 1, 0));
            transformation_matrix = glm::rotate(transformation_matrix, glm::radians(this->rotation.z), glm::vec3(0, 0, 1));

            glm::vec4 finalPosition = transformation_matrix * glm::vec4(1.f);

            glm::vec3 position = glm::vec3(finalPosition);

            GLuint lightAddress = glGetUniformLocation(shaderProgram, "plightPos");
            glUniform3fv(lightAddress, 1, glm::value_ptr(position));
            GLuint lightColorAddress = glGetUniformLocation(shaderProgram, "plightColor");
            glUniform3fv(lightColorAddress, 1, glm::value_ptr(color));
            GLuint lightIntensityAddress = glGetUniformLocation(shaderProgram, "pIntensity");
            glUniform1f(lightIntensityAddress, intensity);
        }

        void setRotation(glm::vec3 rotation) {
            this->rotation = rotation;
        }
    };
}

#endif