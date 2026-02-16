#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Texture.h"
#include "shader_class.h" 
#include "VAO.H"
#include "VBO.H"
#include "EBO.H"
#include "Camera.h"


const unsigned int w = 1080;
const unsigned int h = 720;


GLfloat vertices[] = {
    // VERTICES COORDS            COLOR                UV (position for the texture)
    -0.4f, -0.4f, 0.4f,     1.0f, 0.1f, 0.0f,       0.0f, 1.0f, // front bottom left (0)
    0.4f, -0.4f, 0.4f,      0.1f, 1.0f, 0.0f,       1.0f, 1.0f, // front bottom right (1)
    0.4f, 0.8f, 0.4f,       0.0f, 0.1f, 1.0f,       1.0f, 0.0f, // front top right (2)
    -0.4f, 0.8f, 0.4f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,  // front top left (3)

    -0.4f, -0.4f, -0.4f,     1.0f, 0.1f, 0.0f,       1.0f, 1.0f, // back bottom left (4)
    0.4f, -0.4f, -0.4f,      0.1f, 1.0f, 0.0f,       0.0f, 1.0f, // back bottom right (5)
    0.4f, 0.8f, -0.4f,       0.0f, 0.1f, 1.0f,       0.0f, 0.0f, // back top right (6)
    -0.4f, 0.8f, -0.4f,      1.0f, 1.0f, 1.0f,       1.0f, 0.0f,  // back top left (7)
    
    -0.4f, -0.4f, -0.4f,     1.0f, 0.1f, 0.0f,       1.0f, 1.0f, // left bottom left (8)
    -0.4f, -0.4f, 0.4f,     1.0f, 0.1f, 0.0f,       0.0f, 1.0f, // left bottom left (9)
    -0.4f, 0.8f, 0.4f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,  // left top left (10)
    -0.4f, 0.8f, -0.4f,      1.0f, 1.0f, 1.0f,       1.0f, 0.0f,  // left top left (11)

    0.4f, -0.4f, 0.4f,      0.1f, 1.0f, 0.0f,       1.0f, 1.0f, // right bottom right (12)
    0.4f, -0.4f, -0.4f,      0.1f, 1.0f, 0.0f,       0.0f, 1.0f, // right bottom right (13)
    0.4f, 0.8f, -0.4f,       0.0f, 0.1f, 1.0f,       0.0f, 0.0f, // right top right (14)
    0.4f, 0.8f, 0.4f,       0.0f, 0.1f, 1.0f,       1.0f, 0.0f, // right top right (15)

    -0.4f, -0.4f, 0.4f,     1.0f, 0.1f, 0.0f,       0.0f, 1.0f, // top bottom left (16)
    0.4f, -0.4f, 0.4f,      0.1f, 1.0f, 0.0f,       1.0f, 1.0f, // top bottom right (17)
    0.4f, -0.4f, -0.4f,      0.1f, 1.0f, 0.0f,       1.0f, 0.0f, // top bottom right (18)
    -0.4f, -0.4f, -0.4f,     1.0f, 0.1f, 0.0f,       0.0f, 0.0f, // top bottom left (19)

    -0.4f, 0.8f, 0.4f,      1.0f, 1.0f, 1.0f,       0.0f, 1.0f,  // bottom top left (20)
    0.4f, 0.8f, 0.4f,       0.0f, 0.1f, 1.0f,       1.0f, 1.0f, // bottom top right (21)
    0.4f, 0.8f, -0.4f,       0.0f, 0.1f, 1.0f,       1.0f, 0.0f, // bottom top right (22)
    -0.4f, 0.8f, -0.4f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,  // bottom top left (23)
};

GLuint indices[] = {
    0, 1, 2, // front right triangle
    2, 3, 0, // front left triangle

    12, 13, 14, // right right triang
    14, 15, 12, // right left triang

    4, 5, 6, // back right triang
    6, 7, 4, //

    8, 11, 10, // left right triang
    10, 9, 8,  // left left triang

    16, 17, 18, // bottom right triang
    18, 19, 16, // bottom left triang

    20, 21, 22, // top right triang
    22, 23, 20, // top left triang
};

void Set_version_profile(int major, int minor){
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

int main(){
    glfwInit();

    Set_version_profile(3, 3);

    GLFWwindow *window = glfwCreateWindow(w, h, "Aula7", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, w, h);

    Shader shader_program("resource/shaders/default.vert", 
                          "resource/shaders/default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*) 0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));


    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();


    Texture obj1("resource/textures/marin.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    obj1.texUnit(shader_program, "tex0", 0);



    glEnable(GL_DEPTH_TEST);

    Camera camera (w, h, glm::vec3(0.0f, 0.0f, 2.0f));

    float last_time = glfwGetTime();

    while (!glfwWindowShouldClose(window)){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_program.Activate();

        float current_time = glfwGetTime();
        float delta_time = current_time - last_time;
        last_time = current_time;
 
        camera.Inputs(window, delta_time);
        camera.Matrix(45.0f, 0.1f, 100.0f, shader_program, "camMatrix");

        obj1.Bind();
 
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    

    VAO1.Delete();
    VBO1.Delete(); 
    EBO1.Delete();
    obj1.Delete();
    shader_program.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}