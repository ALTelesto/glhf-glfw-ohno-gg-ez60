#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "math.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define M_PI           3.14159265358979323846

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

float x_mod = 0.0f;
float y_mod = 0.0f;
float z_mod = -5.0f;

float rx_mod = 0.0f;
float ry_mod = 0.0f;

float scale_mod = 1.0f;

float fov_mod = 60.0f;

void Key_Callback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mod
) {
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        x_mod += 0.1;
    }
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        x_mod -= 0.1;
    }
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        y_mod += 0.1;
    }
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        y_mod -= 0.1;
    }
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        rx_mod -= 10.0;
    }
    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        rx_mod += 10.0;
    }
    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        ry_mod -= 10.0;
    }
    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        ry_mod += 10.0;
    }
    if (key == GLFW_KEY_X && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        fov_mod -= 1;
    }
    if (key == GLFW_KEY_Z && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        fov_mod += 1;
    }
    if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        scale_mod -= 0.1;
    }
    if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        scale_mod += 0.1;
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(600, 600, "Hello World", NULL, NULL);
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



    std::string path = "3D/myCube.obj";
    std::vector<tinyobj::shape_t> shape;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shape,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shape[0].mesh.indices[i].vertex_index
        );
    }

    //texture
    std::vector<GLfloat> fullVertexData;
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

        //U
        fullVertexData.push_back(
            attributes.texcoords[vData.texcoord_index * 2]
        );
        //V
        fullVertexData.push_back(
            attributes.texcoords[vData.texcoord_index * 2 + 1]
        );
    }

    GLfloat UV[]{
        0.f, 2.f,
        0.f, 0.f,
        2.f, 2.f,
        2.f, 0.f,
        2.f, 2.f,
        2.f, 0.f,
        0.f, 2.f,
        0.f, 0.f
    };
    
    glfwMakeContextCurrent(window);
    gladLoadGL();
    int img_width, img_height, colorChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* tex_bytes = stbi_load("3D/ayaya.png", &img_width, &img_height, &colorChannels, 0);

    GLuint texture;

    glGenTextures(1, &texture);

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_bytes);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(tex_bytes);
    //texture end

    GLfloat vertices[]{
        //  x    y     z
            0.0, 0.5, 0.0,
            -0.5, -0.5, 0.0,
            0.5f, -0.5, 0.0
    };

    GLuint indices[]{
        0,1,2
    };

    GLuint VAO, VBO, EBO, VBO_UV;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &VBO_UV);
    //glGenBuffers(1, &EBO);

    //text
    glBindVertexArray(VAO);
    //text
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * fullVertexData.size(),
        fullVertexData.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3, //xyz
        GL_FLOAT,
        GL_FALSE,
        //XYZ UV
        5 * sizeof(GL_FLOAT),
        (void*)0
    );

    GLintptr uvPtr = 3 * sizeof(float);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        //XYZ UV
        5 * sizeof(GL_FLOAT),
        (void*)uvPtr
    );

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(2);

    //texture
    /*
    glBindBuffer(GL_ARRAY_BUFFER, VBO_UV);
    glBufferData(GL_ARRAY_BUFFER,
                sizeof(GLfloat) * (sizeof(UV) / sizeof(UV[0])),
                &UV[0],
                GL_DYNAMIC_DRAW);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  
    */
    //texture end

    /*
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW
    );
    */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glm::mat4 identity_matrix = glm::mat4(1.0f);

    glEnable(GL_DEPTH_TEST);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //rabbit pos = 0,0,-5
        //camera pos = 0,0,10
        //camera center = 0,3,0
        glm::vec3 cameraPos = glm::vec3(0, 0, 10.f);
        glm::mat4 cameraPosMatrix = glm::translate(
            glm::mat4(1.0f),
            cameraPos * -1.0f
        );

        glm::vec3 worldUp = glm::normalize(glm::vec3(0, 1.0f, 0));
        glm::vec3 cameraCenter = glm::vec3(0, 3.0f, 0);

        /*
        glm::vec3 F = (cameraCenter - cameraPos);
        F = glm::normalize(F);

        glm::vec3 R = glm::normalize(glm::cross(F, worldUp));
        glm::vec3 U = glm::cross(R, F);

        glm::mat4 cameraOrientation = glm::mat4(1.0f);

        cameraOrientation[0][0] = R.x;
        cameraOrientation[1][0] = R.y;
        cameraOrientation[2][0] = R.z;

        cameraOrientation[0][1] = U.x;
        cameraOrientation[1][1] = U.y;
        cameraOrientation[2][1] = U.z;

        cameraOrientation[0][2] = -F.x;
        cameraOrientation[1][2] = -F.y;
        cameraOrientation[2][2] = -F.z;

        glm::mat4 viewMatrix = cameraOrientation * cameraPosMatrix;
        */

        glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraCenter, worldUp);

        glm::mat4 projection = glm::perspective(glm::radians(fov_mod), 600.0f / 600.0f, 0.1f, 100.0f);

        float x = 0.0;
        float y = 0.0;
        float z = -5.0;
        float scale_x = 1.0;
        float scale_y = 1.0;
        float scale_z = 1.0;
        float radians = 90.0;
        float axis_x = 0.0;
        float axis_y = 1.0;
        float axis_z = 0.0;

        glm::mat4 transformation_matrix = glm::translate(identity_matrix, glm::vec3(x_mod,y_mod,z));

        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(scale_mod, scale_mod, scale_mod));

        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(rx_mod), glm::normalize(glm::vec3(axis_x, axis_y, axis_z)));

        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(ry_mod), glm::normalize(glm::vec3(axis_y, axis_x, axis_z)));

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glUniform1i(tex0Address, 0);


        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        /*
        glDrawElements(
            GL_TRIANGLES, 
            mesh_indices.size(), 
            GL_UNSIGNED_INT, 
            0
        );
        */
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 5);

        /*
        float x, y, nx, ny;
        int i;
        glBegin(GL_POLYGON);

        x = 0.5;
        y = 0.0;

        glVertex2f(x, y);

        
        for (i = 0; i < 5; i++) {
            nx = x * cosf((72.0 * i) * (M_PI / 180)) - y * sinf((72.0 * i) * (M_PI / 180));
            ny = y * cosf((72.0 * i) * (M_PI / 180)) + x * sinf((72.0 * i) * (M_PI / 180));
            glVertex2f(nx,ny);
        }
        

        glEnd();
        */

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}