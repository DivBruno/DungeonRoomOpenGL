#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main()
{
    // initialize glfw
    glfwInit();

    // specify the version used, in this case is 3.3 (major is the left, minor is the right)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // specify the profile, it could be "core" or "compatibility", core has more modern functions.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // just some variables for the resolution, not necessary, but make things organized
    int width = 1080;
    int height = 720;

    // create a variable to hold the window attributes
    GLFWwindow *window = glfwCreateWindow(width, height, "Telinha", NULL, NULL);

    // just making sure if everything went smooth with the window creation
    if (window == NULL){
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // shows the window in the "context"
    glfwMakeContextCurrent(window);

    // initialize glad, it is the one that use openGL (so it actives openGL)
    gladLoadGL();

    // specify the resolution from the window that will be rendered, going from bottom left to top right
    glViewport(0, 0, 1080, 720);

    // specify the colors, but dont use them in the window yet
    glClearColor(0.0, 0.0, 0.0, 1.0);
    // clean the collor from the older buffer
    glClear(GL_COLOR_BUFFER_BIT);
    // swap the back buffer with the front buffer, this  is the one that really gives the color.
    glfwSwapBuffers(window);

    // a loop to make the window display until be close by the user, otherwise, it would open and close a single frame.
    while (!glfwWindowShouldClose(window)){
        // take cares of glfw events, such as changing the window size, closing it, etc.
        glfwPollEvents();
    }

    // deletes the window before closing the program
    glfwDestroyWindow(window);

    // end the glfw before closing the program
    glfwTerminate();

    return 0;
}