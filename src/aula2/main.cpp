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

    GLFWwindow *window = glfwCreateWindow(w, h, "Aula2", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, 1080, 720);

    // Create the vertex shader object
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    // Attach the vertex shader source to the object
    glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL);
    // Compile the vertex shader 
    glCompileShader(vertex_shader);

    // Create the fragment shader object
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attach the fragment shader source to the object
    glShaderSource(fragment_shader, 1, &fragmentShaderSource, NULL);
    // Compile the fragment shader
    glCompileShader(fragment_shader);

    // Create the shader program object
    GLuint shader_program = glCreateProgram();
    //Attach both shaders to the shader program, as if shader program is a list with the shaders.
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    // Link all the shaders together into the shader program
    glLinkProgram(shader_program);

    // Now that everything is inside the shader program, we can delete them, they dont have use anymore.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    /* Vertices coordinates, each 3 is a vertices, 
    the last one being Z (Since is a 2d image, dont have a Z coordinate)*/
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, // bottom left
        0.5f, -0.5f, 0.0f, // bottom right
        0.0f, 0.8f, 0.0f // top
    };


    // Create the variable for the Vertex Array Object & the Vertex Buffer Object
    GLuint VAO, VBO;

    // Generate the VAO
    glGenVertexArrays(1, &VAO);
    // Generate the VBO
    glGenBuffers(1, &VBO);

    /* Bind the VAO (Basically, it says he's the one that the vertex array 
                     functions will refer to)*/
    glBindVertexArray(VAO);
    // Bind the VBO, same reason as the VAO, but for the buffer, specify its a array buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Introduces vertices into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configure the vertex attribute, so the openGL knows how to read the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // Enable the vertex attribute, so the openGL knows how to use it
    glEnableVertexAttribArray(0);

    // Resets the binding, just to not make accidents in the future.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    while (!glfwWindowShouldClose(window)){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Actives the shader program, enabling the object (this case a triangle) to appear
        glUseProgram(shader_program);
        // Bind the VAO
        glBindVertexArray(VAO); 
        // Draw the triangle in the window
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // Deletes all the objects created
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader_program); 

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}