#include "Mesh.h"
#include "tiny_obj_loader.h"

Mesh LoadOBJ(const std::string& path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());
    if (!warn.empty()) std::cout << "WARN: " << warn << std::endl;
    if (!err.empty()) std::cerr << "ERR: " << err << std::endl;
    if (!ret) throw std::runtime_error("Failed to load OBJ");

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};
            vertex.position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            if (!attrib.normals.empty())
                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
            else
                vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);

            if (!attrib.texcoords.empty())
                vertex.texUV = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                };
            else
                vertex.texUV = glm::vec2(0.0f, 0.0f);

            vertex.color = glm::vec3(1.0f, 1.0f, 1.0f); // branco
            vertices.push_back(vertex);
            indices.push_back(indices.size());
        }
    }

    std::vector<Texture> empty_textures;
    return Mesh(vertices, indices, empty_textures);
}

// Window res
const unsigned int w = 1080;
const unsigned int h = 720;


// CUBE, old organization
/*
GLfloat vertices[] = {
    // VERTICES COORDS            COLOR              UV (text pos)            NORMALS 
    -0.4f, -0.4f, 0.4f,     1.0f, 0.1f, 0.0f,       0.0f, 1.0f,         0.0f, 0.0f, 1.0f, // front bottom left (0)  
    0.4f, -0.4f, 0.4f,      0.1f, 1.0f, 0.0f,       1.0f, 1.0f,         0.0f, 0.0f, 1.0f, // front bottom right (1)
    0.4f, 0.8f, 0.4f,       0.0f, 0.1f, 1.0f,       1.0f, 0.0f,         0.0f, 0.0f, 1.0f, // front top right (2)
    -0.4f, 0.8f, 0.4f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f, 0.0f, 1.0f, // front top left (3)

    -0.4f, -0.4f, -0.4f,     1.0f, 0.1f, 0.0f,       1.0f, 1.0f,        0.0f, 0.0f, -1.0f, // back bottom left (4)
    0.4f, -0.4f, -0.4f,      0.1f, 1.0f, 0.0f,       0.0f, 1.0f,        0.0f, 0.0f, -1.0f, // back bottom right (5)
    0.4f, 0.8f, -0.4f,       0.0f, 0.1f, 1.0f,       0.0f, 0.0f,        0.0f, 0.0f, -1.0f, // back top right (6)
    -0.4f, 0.8f, -0.4f,      1.0f, 1.0f, 1.0f,       1.0f, 0.0f,        0.0f, 0.0f, -1.0f, // back top left (7)
    
    -0.4f, -0.4f, -0.4f,     1.0f, 0.1f, 0.0f,       1.0f, 1.0f,        -1.0f, 0.0f, 0.0f, // left bottom left (8)
    -0.4f, -0.4f, 0.4f,     1.0f, 0.1f, 0.0f,       0.0f, 1.0f,         -1.0f, 0.0f, 0.0f, // left bottom left (9)
    -0.4f, 0.8f, 0.4f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         -1.0f, 0.0f, 0.0f, // left top left (10)
    -0.4f, 0.8f, -0.4f,      1.0f, 1.0f, 1.0f,       1.0f, 0.0f,        -1.0f, 0.0f, 0.0f, // left top left (11)

    0.4f, -0.4f, 0.4f,      0.1f, 1.0f, 0.0f,       1.0f, 1.0f,         1.0f, 0.0f, 0.0f, // right bottom right (12)
    0.4f, -0.4f, -0.4f,      0.1f, 1.0f, 0.0f,       0.0f, 1.0f,        1.0f, 0.0f, 0.0f, // right bottom right (13)
    0.4f, 0.8f, -0.4f,       0.0f, 0.1f, 1.0f,       0.0f, 0.0f,        1.0f, 0.0f, 0.0f, // right top right (14)
    0.4f, 0.8f, 0.4f,       0.0f, 0.1f, 1.0f,       1.0f, 0.0f,         1.0f, 0.0f, 0.0f, // right top right (15)

    -0.4f, -0.4f, 0.4f,     1.0f, 0.1f, 0.0f,       0.0f, 1.0f,         0.0f, -1.0f, 0.0f, // top bottom left (16)
    0.4f, -0.4f, 0.4f,      0.1f, 1.0f, 0.0f,       1.0f, 1.0f,         0.0f, -1.0f, 0.0f, // top bottom right (17)
    0.4f, -0.4f, -0.4f,      0.1f, 1.0f, 0.0f,       1.0f, 0.0f,        0.0f, -1.0f, 0.0f, // top bottom right (18)
    -0.4f, -0.4f, -0.4f,     1.0f, 0.1f, 0.0f,       0.0f, 0.0f,        0.0f, -1.0f, 0.0f, // top bottom left (19)

    -0.4f, 0.8f, 0.4f,      1.0f, 1.0f, 1.0f,       0.0f, 1.0f,         0.0f, 1.0f, 0.0f, // bottom top left (20)
    0.4f, 0.8f, 0.4f,       0.0f, 0.1f, 1.0f,       1.0f, 1.0f,         0.0f, 1.0f, 0.0f, // bottom top right (21)
    0.4f, 0.8f, -0.4f,       0.0f, 0.1f, 1.0f,       1.0f, 0.0f,        0.0f, 1.0f, 0.0f, // bottom top right (22)
    -0.4f, 0.8f, -0.4f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,        0.0f, 1.0f, 0.0f, // bottom top left (23)
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
*/


// SQUARE
Vertex vertices[] = {
    //                      COORDS                          NORMALS                         COLORS               UV (text pos)
    Vertex{glm::vec3(-1.0f,  0.0f,  1.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2( 0.0f,  0.0f)},
    Vertex{glm::vec3(-1.0f,  0.0f, -1.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2( 0.0f,  1.0f)},
    Vertex{glm::vec3( 1.0f,  0.0f, -1.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2( 1.0f,  1.0f)},
    Vertex{glm::vec3( 1.0f,  0.0f,  1.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2( 1.0f,  0.0f)}
};


GLuint indices[] = {
    0, 1, 2,
    0, 2, 3
};


Vertex light_vertices[] = {
    Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
    Vertex{glm::vec3( 0.1f, -0.1f, -0.1f)},
    Vertex{glm::vec3( 0.1f, -0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
    Vertex{glm::vec3( 0.1f,  0.1f, -0.1f)},
    Vertex{glm::vec3( 0.1f,  0.1f,  0.1f)}
};


GLuint light_indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};



void Set_version_profile(int major, int minor){
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

int main(){
    glfwInit();

    Set_version_profile(3, 3);

    GLFWwindow *window = glfwCreateWindow(w, h, "OpenGL", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, w, h);

    Texture textures[]{
        Texture ("resource/textures/planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture ("resource/textures/planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
    };


    Shader shader_program("resource/shaders/default.vert", 
                          "resource/shaders/default.frag");
    std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
    std::vector <GLuint> inds(indices, indices + sizeof(indices) /  sizeof(GLuint));
    std::vector <Texture> texs(textures, textures + sizeof(textures) / sizeof(Texture));
    Mesh floor(verts, inds, texs);

    Shader light_shader("resource/shaders/light.vert", 
                        "resource/shaders/light.frag");
    std::vector <Vertex> light_verts(light_vertices, light_vertices + sizeof(light_vertices) / sizeof(Vertex));
    std::vector <GLuint> light_inds(light_indices, light_indices + sizeof(light_indices) / sizeof(GLuint));
    Mesh light(light_verts, light_inds, texs);

    // Aq tu muda a textura
    std::vector<Texture> modelTextures{
        Texture("resource/textures/planks.png",
                "diffuse",
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE)
    };

    Mesh myModel = LoadOBJ("resource/models/girl.obj");
    myModel.textures = modelTextures;

    glm::vec3 model_pos = glm::vec3(10.0f, 0.0f, -1.0f);
    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, model_pos);



    glm::vec4 light_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 light_pos = glm::vec3(0.8f, 0.8f, 0.8f);
    glm::mat4 light_model = glm::mat4(1.0f);
    light_model = glm::translate(light_model, light_pos);

    glm::vec3 cube_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 cube_model = glm::mat4(1.0f);
    cube_model = glm::translate(cube_model, cube_pos);
    

    light_shader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(light_shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(light_model));
    glUniform4f(glGetUniformLocation(light_shader.ID, "light_color"), light_color.x, light_color.y, light_color.z, light_color.w);
    shader_program.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader_program.ID, "model"), 1, GL_FALSE, glm::value_ptr(cube_model));
    glUniform4f(glGetUniformLocation(shader_program.ID, "light_color"), light_color.x, light_color.y, light_color.z, light_color.w);
    glUniform3f(glGetUniformLocation(shader_program.ID, "light_pos"), light_pos.x, light_pos.y, light_pos.z);

    glEnable(GL_DEPTH_TEST);

    Camera camera (w, h, glm::vec3(0.0f, 0.0f, 2.0f));

    float last_time = glfwGetTime();

    while (!glfwWindowShouldClose(window)){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        float current_time = glfwGetTime();
        float delta_time = current_time - last_time;
        last_time = current_time;

        
        camera.Inputs(window, delta_time);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // ---- MODELO ----
        shader_program.Activate();
        glUniformMatrix4fv(
            glGetUniformLocation(shader_program.ID, "model"),
                                1, GL_FALSE, glm::value_ptr(model_matrix)
        );
        myModel.Draw(shader_program, camera);

        // ---- CHÃO ----
        glUniformMatrix4fv(
            glGetUniformLocation(shader_program.ID, "model"),
                                1, GL_FALSE, glm::value_ptr(cube_model) // matriz original do chão
        );
        floor.Draw(shader_program, camera);

        // ---- LUZ ----
        light.Draw(light_shader, camera);     
            

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shader_program.Delete();
    light_shader.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}