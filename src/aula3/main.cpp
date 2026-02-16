#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Shaders sources, ignorar no momento, será explicado melhor depois.
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.8, 0.0f, 0.0f, 0.5f);\n"
"}\n\0";


int main(){
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int w = 1080;
    int h = 720;

    GLFWwindow *window = glfwCreateWindow(w, h, "Aula3", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, 1080, 720);

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragment_shader);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, // bottom left (0)
        0.5f, -0.5f, 0.0f, // bottom right (1)
        0.0f, 1.0f, 0.0f, // top (2)
        -0.5f/2, 0.5f/2, 0.0f, // inner left (5)
        0.5f/2, 0.5f/2, 0.0f, //  inner  right (4)
        0.0f, -0.5, 0.0f // inner down (3)
    };

    /* It is a way to create multiple triangles, but without wasting memory by creating repetitive vertices, 
    so if a triangle has the same vertice as another triangle, instead of giving them 2 equals vertices, 
    just create a vertice and give the indices of these vertices
    */
    GLuint indices[] = {
        0, 3, 5, // lower  left triangle
        3, 2, 4, // lower right triangle
        5, 4, 1  // top triangle
    };

    // Just like with VAO & VBO, we need to create a reference to EBO (Element Buffer Object), this buffer is for the indexes we created
    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Generate the EBO, indicating it is only 1 object
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Bind the EBO, basically saying he's the EBO we'll be referencing  to in the functinoos about EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Give the indices to the buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // now unbind it, just in case. OBS: It needs to be unbinded AFTER the VAO!
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    while (!glfwWindowShouldClose(window)){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(VAO); 
        /* it was changed, from "draw arrays" to "draw elements". Guess it makes the program knows we're talking about indices,
        Being necessary specify how many indices, the data type they are, and the index.
        */  
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // Deleting the buffer before ending the program.
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader_program); 

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}