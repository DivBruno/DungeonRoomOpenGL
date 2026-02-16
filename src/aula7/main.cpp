#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
 
#include "shader_class.h" 
#include "VAO.H"
#include "VBO.H"
#include "EBO.H"



GLfloat vertices[] = {
    -0.4f, -0.4f, 0.0f,     1.0f, 0.1f, 0.0f,       0.0f, 1.0f, // bottom left (0)
    0.4f, -0.4f, 0.0f,      0.1f, 1.0f, 0.0f,       1.0f, 1.0f, // bottom right (1)
    0.4f, 0.55f, 0.0f,       0.0f, 0.1f, 1.0f,       1.0f, 0.0f, // top right (2)
    -0.4f, 0.55f, 0.0f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f // top left (3)
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

    GLFWwindow *window = glfwCreateWindow(w, h, "Aula7", NULL, NULL);
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

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*) 0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));


    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    GLuint uniID = glGetUniformLocation(shader_program.ID, "scale");

    // Texture attributes, width, height and number of color channels
    int img_width, img_height, img_num_col_ch;
    // reads the image and stores it
    unsigned char* bytes = stbi_load("resource/textures/31.jpg", &img_width, &img_height, &img_num_col_ch, STBI_rgb_alpha);


    // Generates an texture object
    GLuint texture;
    glGenTextures(1, &texture);
    // Assigns the texture to a texture unit
    glActiveTexture(GL_TEXTURE0);
    // Binds the the texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // Configures the way the texture will be renderized, can be nearest ou linear. The first makes it be more pixelated, better to pixel arts; 
    //                                                                              the second one makes it more blurry.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Configures the way the texture repeats (if it'll be a lot of the same imagens, mirrored images, 
    //                                                              or just make a border flat color, it is the one i chose)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // Lines since i chose the clamp_to_border, it gives the border a color, the chosen one is white
    float back_color[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, back_color);

    // ASsigns the image to the texture object
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    // Generate mipmaps (alternative resolutions to the image)
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // Deletes the image data, since it is already in a texture object
    stbi_image_free(bytes);

    // Unbinds the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // Assigns a texture unit to a texture
    GLuint tex0_uni = glGetUniformLocation(shader_program.ID, "tex0");
    shader_program.Activate();
    glUniform1i(tex0_uni, 0);


    while (!glfwWindowShouldClose(window)){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_program.Activate();
        glUniform1f(uniID, 0.8f);
        glBindTexture(GL_TEXTURE_2D, texture);
 
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    // Deletes the texture
    glDeleteTextures(1, &texture);
    shader_program.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}