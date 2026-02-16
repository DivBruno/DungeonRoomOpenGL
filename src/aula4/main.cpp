#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// organizing the project, mainly making the shaders and buffers as separate classes
#include "shader_class.h" 
#include "VAO.H"
#include "VBO.H"
#include "EBO.H"


// just made the variables global.
GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f, // bottom left (0)
    0.5f, -0.5f, 0.0f, // bottom right (1)
    0.0f, 1.0f, 0.0f, // top (2)
    
    -0.5f/2, 0.5f/2, 0.0f, // inner left (5)
    0.5f/2, 0.5f/2, 0.0f, //  inner  right (4)
    0.0f, -0.5, 0.0f // inner down (3)
};

GLuint indices[] = {
    0, 3, 5, // lower  left triangle
    3, 2, 4, // lower right triangle
    5, 4, 1  // top triangle
};

// just made a function to select the version and profile
void set_version_profile(int major, int minor){
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

int main(){
    glfwInit();

    set_version_profile(3, 3);

    int w = 1080;
    int h = 720;

    GLFWwindow *window = glfwCreateWindow(w, h, "Aula4", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, 1080, 720);

    // Created a new class "Shader", it'll take care of vertex shaders, fragment shaders and shader program.
    // Now the vertex and fragment shaders are in separated files, in resource/shaders.
    Shader shader_program("resource/shaders/default.vert", 
                          "resource/shaders/default.frag");

    // creates the VAO and binds it
    VAO VAO1;
    VAO1.Bind();

    // creates the VBO and the EBO, bind them as well.
    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    // links the VBO and VAO
    VAO1.LinkVBO(VBO1, 0);

    // unbinds everything after they're used. Just a precaution.
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    while (!glfwWindowShouldClose(window)){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Actives the shader program
        shader_program.Activate();
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    

    // Deleting the buffers & shader prgram before ending the program.
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shader_program.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}