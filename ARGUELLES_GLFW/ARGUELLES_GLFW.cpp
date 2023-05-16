#include "GLFW/glfw3.h"
#include "math.h"

# define M_PI           3.14159265358979323846

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

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

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

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}