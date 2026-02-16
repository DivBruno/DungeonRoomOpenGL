#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_class.h" 
#include "VAO.H"
#include "VBO.H"
#include "EBO.H"



GLfloat vertices[] = {
    -0.4f, -0.4f, 0.0f,     0.1f, 0.1f, 1.0f, // bottom left (0)
    0.4f, -0.4f, 0.0f,      0.1f, 0.1f, 1.0f, // bottom right (1)
    0.4f, 0.4f, 0.0f,       1.0f, 0.1f, 0.1f, // top right (2)
    -0.4f, 0.4f, 0.0f,      1.0f, 0.1f, 0.1f // top left (3)
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

    GLFWwindow *window = glfwCreateWindow(w, h, "Aula6", NULL, NULL);
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

    // now is able to link more than 1 attribute, in this case is vertices positions and colors, respectively 
    // OBS: in linkAttrib is neccessary to change the 2° parameter and the last ones.
    //      the 2° parameter is basicaly to say "this attribute is the 1° attribute (0), the 2° attribute (1).""
    //          you define the location of the attribute in the default.vert, that is why the position is '0' and color is '1'
    //      the last one you're telling the VBO that after x bytes, is the next attribute
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*) 0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));


    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Gets ID of uniform called scale, scale is inside default.vert
    GLuint uniID = glGetUniformLocation(shader_program.ID, "scale");


    while (!glfwWindowShouldClose(window)){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_program.Activate();
        // assigns a value to the uniform, it must be done after activating the shader program.
        glUniform1f(uniID, 0.5f);
 
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