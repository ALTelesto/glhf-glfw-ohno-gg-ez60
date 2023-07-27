#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "iostream"
#include "math.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

//#include "Camera.hpp"
//#include "Light.hpp"
//#include "Model.hpp"

#define M_PI           3.14159265358979323846

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
using namespace camera;

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
        DirectionalLight(glm::vec3 position, glm::vec3 color, float intensity) : Light3D(position, color, intensity){
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
            this->rotation = glm::vec3(0.f,0.f,0.f);
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
using namespace light;

namespace model {
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
        Model3D(std::string pathModel, std::string pathTexture, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 color) {
            this->position = position;
            this->rotation = rotation;
            this->scale = scale;
            this->color = color;
            this->specStr = 0;
            this->specPhong = 0;

            //std::string path = "3D/djSword.obj";
            std::vector<tinyobj::shape_t> shape;
            std::vector<tinyobj::material_t> material;
            std::string warning, error;

            tinyobj::attrib_t attributes;

            /*
            if (!pathModel.empty()) {
                std::cout << "invalid path" << std::endl;
                return;
            }
            */

            bool success = tinyobj::LoadObj(
                &attributes,
                &shape,
                &material,
                &warning,
                &error,
                pathModel.c_str()
            );

            if (!success) {
                std::cout << "failed loading model" << std::endl;
                return;
            }

            for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
                tinyobj::index_t vData = shape[0].mesh.indices[i];
                //X
                fullVertexData.push_back(
                    attributes.vertices[vData.vertex_index * 3]
                );
                //Y
                fullVertexData.push_back(
                    attributes.vertices[vData.vertex_index * 3 + 1]
                );
                //Z
                fullVertexData.push_back(
                    attributes.vertices[vData.vertex_index * 3 + 2]
                );

                fullVertexData.push_back(
                    attributes.normals[vData.normal_index * 3]
                );
                fullVertexData.push_back(
                    attributes.normals[vData.normal_index * 3 + 1]
                );
                fullVertexData.push_back(
                    attributes.normals[vData.normal_index * 3 + 2]
                );

                //U
                fullVertexData.push_back(
                    attributes.texcoords[vData.texcoord_index * 2]
                );
                //V
                fullVertexData.push_back(
                    attributes.texcoords[vData.texcoord_index * 2 + 1]
                );

            }

            /*
            if (!pathTexture.empty()) {
                std::cout << "invalid texture path" << std::endl;
                return;
            }
            else {
                int img_width, img_height, colorChannels;

                stbi_set_flip_vertically_on_load(true);
                unsigned char* tex_bytes = stbi_load(pathTexture.c_str(), &img_width, &img_height, &colorChannels, 0);

                glGenTextures(1, &texture);

                glActiveTexture(GL_TEXTURE0);

                glBindTexture(GL_TEXTURE_2D, texture);

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes);

                glGenerateMipmap(GL_TEXTURE_2D);

                glEnable(GL_DEPTH_TEST);

                stbi_image_free(tex_bytes);
            }
            */

            int img_width, img_height, colorChannels;

            stbi_set_flip_vertically_on_load(true);
            unsigned char* tex_bytes = stbi_load(pathTexture.c_str(), &img_width, &img_height, &colorChannels, 0);

            glGenTextures(1, &texture);

            glActiveTexture(GL_TEXTURE0);

            glBindTexture(GL_TEXTURE_2D, texture);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes);

            glGenerateMipmap(GL_TEXTURE_2D);

            glEnable(GL_DEPTH_TEST);

            stbi_image_free(tex_bytes);


            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(
                GL_ARRAY_BUFFER,
                sizeof(GLfloat) * fullVertexData.size(),
                fullVertexData.data(),
                // attributes.vertices.data(),
                GL_DYNAMIC_DRAW);

            //model
            glVertexAttribPointer(
                0,
                3, //xyz
                GL_FLOAT,
                GL_FALSE,
                //XYZ UV
                8 * sizeof(GL_FLOAT),
                (void*)0
            );

            //normal
            GLintptr normalPtr = 3 * sizeof(float);
            glVertexAttribPointer(
                1,
                3,
                GL_FLOAT,
                GL_FALSE,
                8 * sizeof(GL_FLOAT),
                (void*)normalPtr
            );

            //texture
            GLintptr uvPtr = 6 * sizeof(float);
            glVertexAttribPointer(
                2,
                2,
                GL_FLOAT,
                GL_FALSE,
                //XYZ UV
                8 * sizeof(GL_FLOAT),
                (void*)uvPtr
            );

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);

        }

        // code was ripped from the while() loop in main() and placed into draw() function
        void draw(GLuint& shaderProgram) {

            glUseProgram(shaderProgram);

            glm::mat4 identity_matrix = glm::mat4(1.0f);

            // applying of model transformation
            // translate -> rotate -> scale
            glm::mat4 transformation_matrix = glm::translate(glm::mat4(1.0f), this->position);

            transformation_matrix = glm::rotate(transformation_matrix, glm::radians(this->rotation.x), glm::vec3(1, 0, 0));
            transformation_matrix = glm::rotate(transformation_matrix, glm::radians(this->rotation.y), glm::vec3(0, 1, 0));
            transformation_matrix = glm::rotate(transformation_matrix, glm::radians(this->rotation.z), glm::vec3(0, 0, 1));

            transformation_matrix = glm::scale(transformation_matrix, this->scale);
            // end of applying of model transformation 

            // sending transformation matrix to shader program
            unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

            //set texture
            GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glUniform1i(tex0Address, 0);

            //set color
            unsigned int colorLoc = glGetUniformLocation(shaderProgram, "FragColorIn");
            glUniform4fv(colorLoc, 1, glm::value_ptr(glm::vec4(color, 1.f)));
            // end of sending stuff to shader program

            // rendering
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 8);
            // end
        }

        void clean() {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
        }

        void setPosition(glm::vec3 position) {
            this->position = position;
        }
        void setRotation(glm::vec3 rotation) {
            this->rotation = rotation;
        }
        void setScale(glm::vec3 scale) {
            this->scale = scale;
        }
        void setColor(glm::vec3 color) {
            this->color = color;
        }
        void setSpec(glm::vec2 spec) {
            this->specStr = spec.x;
            this->specPhong = spec.y;
        }

        glm::vec3 getPosition() {
            return this->position;
        }
        glm::vec3 getRotation() {
            return this->rotation;
        }
    };
}
using namespace model;

bool modelToggle = false;
bool cameraToggle = true;

float x_mod = 0.0f;
float y_mod = 0.0f;
float z_mod = -10.0f;

float rx_mod = 0.0f;
float ry_mod = 0.0f;

float light_mod = 0.2f;
float speed_mod = 2.f;
float light_speed_mod = 2.f;

//light distance
float light_dist = 7.f;

glm::vec3 lightOrbitRot = glm::vec3(0.f, 0.f, 0.f);

//camera distance
float cam_dist = 25.f;

float sens_mod = 0.01f;  // camera sensitivity

bool start = true;
float mouseX = 0.0f;
float mouseY = 0.0f;

float scale_mod = 2.f;

float fov_mod = 60.0f;

OrthoCamera* cam0;
PerspectiveCamera* cam1;

DirectionalLight* light0;
PointLight3D* light1;

Model3D* model0;
Model3D* model1;

void fixRotation() {
    if (lightOrbitRot.x > 360) {
        lightOrbitRot.x = 0;
    }
    if (lightOrbitRot.x < 0) {
        lightOrbitRot.x = 360;
    }
    if (lightOrbitRot.y > 360) {
        lightOrbitRot.y = 0;
    }
    if (lightOrbitRot.y < 0) {
        lightOrbitRot.y = 360;
    }
    if (lightOrbitRot.z > 360) {
        lightOrbitRot.z = 0;
    }
    if (lightOrbitRot.z < 0) {
        lightOrbitRot.z = 360;
    }
}

void Key_Callback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mod
) {
    glm::vec3 model0Pos = model0->getPosition();
    glm::vec3 model1Pos = model1->getPosition();

    glm::vec3 model0Rot = model0->getRotation();
    glm::vec3 model1Rot = model1->getRotation();

    float dlight = light0->getIntensity();
    float plight = light1->getIntensity();

    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        model0Rot.y += speed_mod;
        if (!modelToggle) {
            model0->setRotation(model0Rot);
        }
        else {
            lightOrbitRot.y -= light_speed_mod;
            model1->setRotation(model1Rot);
            //light1->setRotation(model1Rot);
        }
    }
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        model0Rot.y -= speed_mod;
        if (!modelToggle) {
            model0->setRotation(model0Rot);
        }
        else {
            lightOrbitRot.y += light_speed_mod;
            model1->setRotation(model1Rot);
            //light1->setRotation(model1Rot);
        }
    }
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        model0Rot.x -= speed_mod;
        if (!modelToggle) {
            model0->setRotation(model0Rot);
        }
        else {
            lightOrbitRot.x -= light_speed_mod;
            model1->setRotation(model1Rot);
            //light1->setRotation(model1Rot);
        }
    }
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        model0Rot.x += speed_mod;
        if (!modelToggle) {
            model0->setRotation(model0Rot);
        }
        else {
            lightOrbitRot.x += light_speed_mod;
            //model1->setRotation(model1Rot);
            //light1->setRotation(model1Rot);
        }
    }
    if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        model0Rot.z += speed_mod;
        if (!modelToggle) {
            model0->setRotation(model0Rot);
        }
        else {
            lightOrbitRot.z += light_speed_mod;
            //model1->setRotation(model1Rot);
            //light1->setRotation(model1Rot);
        }
    }
    if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        model0Rot.z -= speed_mod;
        if (!modelToggle) {
            model0->setRotation(model0Rot);
        }
        else {
            lightOrbitRot.z -= light_speed_mod;
            //model1->setRotation(model1Rot);
            //light1->setRotation(model1Rot);
        }
    }
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        if (modelToggle) {
            light0->setIntensity(dlight + light_mod);
            //std::cout << dlight << std::endl;
        }
    }
    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        if (modelToggle && dlight > 0) {
            light0->setIntensity(dlight - light_mod);
            //std::cout << dlight << std::endl;
        }
    }
    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        if (modelToggle) {
            light1->setIntensity(plight + light_mod);
        }
    }
    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        if (modelToggle && plight > 0) {
            light1->setIntensity(plight - light_mod);
        }
    }
    if (key == GLFW_KEY_1 && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        cameraToggle = true;
    }
    if (key == GLFW_KEY_2 && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        cameraToggle = false;
    }
    if (key == GLFW_KEY_SPACE && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        modelToggle = !modelToggle;
        if (modelToggle) {
            model1->setColor(glm::vec3(0.0f,1.f,0.0f));
            //light0->setColor(glm::vec3(0.0f, 1.f, 0.0f));
            light1->setColor(glm::vec3(0.0f, 1.f, 0.0f));
        }
        else {
            model1->setColor(glm::vec3(1.f, 1.f, 1.f));
            //light0->setColor(glm::vec3(1.f, 1.f, 1.f));
            light1->setColor(glm::vec3(1.f, 1.f, 1.f));
        }
    }

    //float rAngle = atan2f(model1Rot.y, model1Rot.x);

    fixRotation();

    float yaw = glm::radians(lightOrbitRot.y);
    float pitch = glm::radians(lightOrbitRot.x);
    float roll = glm::radians(lightOrbitRot.z);

    glm::quat orientation = glm::quat(glm::vec3(pitch, yaw, roll));
    glm::vec3 position = orientation * glm::vec3(0.f, 0.f, -1.f);
    position = position * light_dist;

    model1->setPosition(position);
    model1->setRotation(lightOrbitRot);

    //std::cout << "!!" << model1Rot.x << ", " << model1Rot.x << ", " << model1Rot.z << std::endl;
    //std::cout << x << ", " << y << ", " << z << std::endl;
}

void Mouse_Callback(
    GLFWwindow* window,
    double mouseXNew,
    double mouseYNew
) {
    if (cameraToggle) {
        // initialize mouse position
        if (start) {
            mouseX = mouseXNew;
            mouseY = mouseYNew;
            start = false;
        }
        // get difference in mouse position
        float offsetX = mouseXNew - mouseX;
        float offsetY = mouseY - mouseYNew;
        // set current position to the new position
        mouseX = mouseXNew;
        mouseY = mouseYNew;

        // multiply with sensitivity modifier
        offsetX *= -sens_mod;
        offsetY *= sens_mod;

        offsetY = glm::clamp(offsetY, -89.f, 89.f);

        glm::vec3 position = cam1->getPosition();
        glm::vec3 center = glm::vec3(0.f, 0.f, 0.f);

        glm::vec3 front = glm::normalize(position - center);
        glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), front));
        glm::vec3 up = glm::cross(front, right);



        glm::mat4 xRot = glm::rotate(glm::mat4(1.0f), offsetX, up);
        position = glm::vec3(xRot * glm::vec4(position, 1.f));

        glm::mat4 yRot = glm::rotate(glm::mat4(1.0f), offsetY, right);
        glm::vec4 newPos = yRot * glm::vec4(position, 1.0f);
        position = glm::vec3(newPos.x, newPos.y, newPos.z);

        cam1->setPosition(position);
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(600, 600, "Joachim Arguelles Programming Challenge 2", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    //glm::mat4 projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -1.0f, 1.0f);

    //glm::mat4 projection = glm::perspective(glm::radians(60.0f), 600.0f / 600.0f, 0.1f, 100.0f);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();


    glfwSetKeyCallback(window, Key_Callback);
    glfwSetCursorPosCallback(window, Mouse_Callback);

    //shaders

    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &v, NULL);
    glCompileShader(vertShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);

    glLinkProgram(shaderProgram);
    

    //unlit shader
    std::fstream unlit_vertSrc("Shaders/sample.vert");
    std::stringstream unlit_vertBuff;
    unlit_vertBuff << unlit_vertSrc.rdbuf();
    std::string unlit_vertS = unlit_vertBuff.str();
    const char* unlit_v = unlit_vertS.c_str();

    std::fstream unlit_fragSrc("Shaders/unlit.frag");
    std::stringstream unlit_fragBuff;
    unlit_fragBuff << unlit_fragSrc.rdbuf();
    std::string unlit_fragS = unlit_fragBuff.str();
    const char* unlit_f = unlit_fragS.c_str();

    GLuint unlit_vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(unlit_vertShader, 1, &unlit_v, NULL);
    glCompileShader(unlit_vertShader);

    GLuint unlit_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(unlit_fragShader, 1, &unlit_f, NULL);
    glCompileShader(unlit_fragShader);

    GLuint unlitshaderProgram = glCreateProgram();
    glAttachShader(unlitshaderProgram, unlit_vertShader);
    glAttachShader(unlitshaderProgram, unlit_fragShader);

    glLinkProgram(unlitshaderProgram);

    //shaders

    //                         cam_dist/100 because camera doesnt like it when its 0.f
    cam0 = new OrthoCamera(glm::vec3(0.f, cam_dist, cam_dist/100), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 60.0);
    cam1 = new PerspectiveCamera(glm::vec3(0.f, 0.f, cam_dist), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 60.0);

    light0 = new DirectionalLight(glm::vec3(5.f, -10.f, -1.f), glm::vec3(0.5f, 0.5f, 0.5f), 0.25);

    //main model
    //model source: roblox avatar, exported from roblox
    model0 = new Model3D("3D/peanut.obj", "3D/peanut.jpg", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(scale_mod, scale_mod, scale_mod), glm::vec3(1.f, 1.f, 1.f));

    //light model
    //model source: blender (default cube)
    model1 = new Model3D("3D/blenderdefaultcube.obj", "3D/partenza.jpg", glm::vec3(0.f, 0.f, -light_dist), glm::vec3(0.001f, 0.f, 0.001f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f));
    light1 = new PointLight3D(model1->getPosition(), glm::vec3(1.f, 1.f, 1.f), 10.0);

    float ambientStr = 0.2f;
    glm::vec3 ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::vec3 positioncam;
        glm::mat4 projection;

        if (!cameraToggle) {
            cam0->draw(shaderProgram);
            positioncam = cam0->getPosition();
            projection = cam0->getProjection();
        }
        else {
            cam1->draw(shaderProgram);
            positioncam = cam1->getPosition();
            projection = cam1->getProjection();
        }

        

        light0->draw(shaderProgram);

        light1->setPosition(model1->getPosition());

        light1->draw(shaderProgram);

        unsigned int ambientColorLoc = glGetUniformLocation(shaderProgram, "ambientColor");
        glUniform3fv(ambientColorLoc, 1, glm::value_ptr(ambientColor));

        unsigned int ambientStrLoc = glGetUniformLocation(shaderProgram, "ambientStr");
        glUniform1f(ambientStrLoc, ambientStr);

        model0->draw(shaderProgram);
        
        if (!cameraToggle) {
            cam0->draw(unlitshaderProgram);
        }
        else {
            cam1->draw(unlitshaderProgram);
        }
        model1->draw(unlitshaderProgram);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    model0->clean();
    model1->clean();

    glfwTerminate();
    return 0;
}