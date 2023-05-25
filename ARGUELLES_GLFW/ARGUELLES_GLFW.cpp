#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "math.h"

#define M_PI           3.14159265358979323846

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

float x_mod = 0.0;
float y_mod = 0.0;

void Key_Callback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mod
) {
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        x_mod += 0.1;
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        x_mod -= 0.1;
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        y_mod += 0.1;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        y_mod -= 0.1;
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(480, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glfwSetKeyCallback(window, Key_Callback);

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

    std::string path = "../3D/bunny.obj";
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

    GLfloat vertices[]{
        //  x    y     z
            0.0, 0.5, 0.0,
            -0.5, -0.5, 0.0,
            0.5f, -0.5, 0.0
    };

    GLuint indices[]{
        0,1,2
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //text
    glBindVertexArray(VAO);
    //text
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * attributes.vertices.size(),
        &attributes.vertices[0],
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3, //xyz
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(GL_FLOAT),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        unsigned int xLoc = glGetUniformLocation(shaderProgram, "x");
        glUniform1f(xLoc, x_mod);

        unsigned int yLoc = glGetUniformLocation(shaderProgram, "y");
        glUniform1f(yLoc, y_mod);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(
            GL_TRIANGLES, 
            mesh_indices.size(), 
            GL_UNSIGNED_INT, 
            0
        );

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