#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_class.h" 
#include "VAO.H"
#include "VBO.H"
#include "EBO.H"



GLfloat vertices[] = {
    -0.4f, -0.4f, 0.0f,     0.8f, 0.3f, 0.02f, // bottom left (0)
    0.4f, -0.4f, 0.0f,      0.8f, 0.3f, 0.02f, // bottom right (1)
    0.4f, 0.8f, 0.0f,       1.0f, 0.6f, 0.32f, // top right (2)
    -0.4f, 0.8f, 0.0f,      0.9f, 0.45f, 0.18f // top left (3)
};

GLuint indices[] = {
    0, 1, 2, // right triangle
    2, 3, 0, // left triangle
};



void Set_version_profile(int major, int minor){
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

int main(){
    glfwInit();

    Set_version_profile(3, 3);

    int w = 1080;
    int h = 720;

    GLFWwindow *window = glfwCreateWindow(w, h, "Aula5", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, 1080, 720);

    Shader shader_program("resource/shaders/default.vert", 
                          "resource/shaders/default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkVBO(VBO1, 0);

    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    float r = 0.0, b = 0.0, g = 0.0, a = 0.0;
    float increment = 0.00005;
    while (!glfwWindowShouldClose(window)){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_program.Activate();
        int location = glGetUniformLocation(shader_program.ID, "u_color");
        glUniform4f(location, r, 0.1f, 0.2f, 1.0f);

        r += increment;
        if (r > 1.0f){
            increment *= -1;
        }
        if (r < 0.0f){
            increment *= -1;
        }

        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shader_program.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}