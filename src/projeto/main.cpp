#include "Mesh.h"
#include "tiny_obj_loader.h"

template <size_t v, size_t i,  size_t t>

Mesh create_object(Vertex (&vertices)[v], GLuint (&indices)[i], Texture (&texture)[t]){
    std::vector <Vertex> verts(vertices, vertices + v);
    std::vector <GLuint> inds(indices, indices + i);
    std::vector <Texture> texs(texture, texture + t);

    Mesh mesh(verts, inds, texs);   
    return mesh;
}   


Mesh LoadOBJ(const std::string& path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), nullptr, true);
    if (!warn.empty() && warn.find("Material") == std::string::npos)
        std::cout << "WARN: " << warn << std::endl;
    if (!err.empty())
        std::cerr << "ERR: " << err << std::endl;
    if (!ret)
        throw std::runtime_error("Failed to load OBJ");

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

    return Mesh(vertices, indices);
}


// Window res
const unsigned int w = 1080;
const unsigned int h = 720;


// OBJECTS VERTICES AND INDICES

//      FLOOR
Vertex vertices_floor[] = {
    //                      COORDS                          NORMALS                         COLORS               UV (text pos)
    Vertex{glm::vec3(-1.0f,  0.0f,  1.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2( 0.0f,  0.0f)},
    Vertex{glm::vec3(-1.0f,  0.0f, -1.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2( 0.0f,  1.0f)},
    Vertex{glm::vec3( 1.0f,  0.0f, -1.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2( 1.0f,  1.0f)},
    Vertex{glm::vec3( 1.0f,  0.0f,  1.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2( 1.0f,  0.0f)}
};

GLuint indices_floor[] = {
    0, 1, 2,
    0, 2, 3
};


//      WALL
Vertex vertices_wall[] = {
    //                      COORDS                          NORMALS                         COLORS               UV (text pos)
    Vertex{glm::vec3(-1.0f,  1.0f,  0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2( 0.0f,  0.0f)},
    Vertex{glm::vec3(-1.0f,  -1.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2( 0.0f,  1.0f)},
    Vertex{glm::vec3( 1.0f,  -1.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2( 1.0f,  1.0f)},
    Vertex{glm::vec3( 1.0f,  1.0f,  0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2( 1.0f,  0.0f)}
};

GLuint indices_wall[] = {
    0, 1, 2,
    0, 2, 3
};



float rx = 0.2, ry = 0.02, rz = 0.5;
//      RUG 
Vertex vertices_rug[] = {
    Vertex{glm::vec3(-rx, 0.0f, -rz), glm::vec3(1.0f,  0.1f,  0.0f), glm::vec3( -1.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  1.0f)}, 
    Vertex{glm::vec3(-rx, 0.0f,  rz), glm::vec3(1.0f,  0.1f,  0.0f), glm::vec3( -1.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  1.0f)}, 
    Vertex{glm::vec3(-rx,   ry,  rz), glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3( -1.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  0.0f)}, 
    Vertex{glm::vec3(-rx,   ry, -rz), glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3( -1.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  0.0f)}, 

    Vertex{glm::vec3( rx, 0.0f,  rz), glm::vec3(0.1f,  1.0f,  0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  1.0f)}, 
    Vertex{glm::vec3( rx, 0.0f, -rz), glm::vec3(0.1f,  1.0f,  0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  1.0f)},
    Vertex{glm::vec3( rx,   ry, -rz), glm::vec3(0.0f,  0.1f,  1.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  0.0f)},
    Vertex{glm::vec3( rx,   ry,  rz), glm::vec3(0.0f,  0.1f,  1.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  0.0f)}, 

    Vertex{glm::vec3(-rx, 0.0f,  rz), glm::vec3(1.0f,  0.1f,  0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2( 0.0f,  1.0f)},
    Vertex{glm::vec3( rx, 0.0f,  rz), glm::vec3(0.1f,  1.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2( 1.0f,  1.0f)},
    Vertex{glm::vec3( rx,   ry,  rz), glm::vec3(0.0f,  0.1f,  1.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2( 1.0f,  0.0f)}, 
    Vertex{glm::vec3(-rx,   ry,  rz), glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2( 0.0f,  0.0f)},   

    Vertex{glm::vec3(-rx, 0.0f, -rz), glm::vec3(1.0f,  0.1f,  0.0f), glm::vec3( 0.0f,  0.0f,  -1.0f), glm::vec2( 1.0f,  1.0f)},
    Vertex{glm::vec3( rx, 0.0f, -rz), glm::vec3(0.1f,  1.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  -1.0f), glm::vec2( 0.0f,  1.0f)},
    Vertex{glm::vec3( rx,   ry, -rz), glm::vec3(0.0f,  0.1f,  1.0f), glm::vec3( 0.0f,  0.0f,  -1.0f), glm::vec2( 0.0f,  0.0f)}, 
    Vertex{glm::vec3(-rx,   ry, -rz), glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3( 0.0f,  0.0f,  -1.0f), glm::vec2( 1.0f,  0.0f)},
};

Vertex vertices_rug_top[] = {
    Vertex{glm::vec3(-rx,   ry,  rz), glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2( 0.0f,  1.0f)}, 
    Vertex{glm::vec3( rx,   ry,  rz), glm::vec3(0.0f,  0.1f,  1.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2( 1.0f,  1.0f)}, 
    Vertex{glm::vec3( rx,   ry, -rz), glm::vec3(0.0f,  0.1f,  1.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2( 1.0f,  0.0f)}, 
    Vertex{glm::vec3(-rx,   ry, -rz), glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2( 0.0f,  0.0f)},
};
                            


//      BOOKSHELF
float x = 0.25, y = 1.6, z = 0.5;
Vertex vertices_bookshelf[] = {
    Vertex{glm::vec3(-x, 0.0f,-z), glm::vec3(1.0f,  0.1f,  0.0f), glm::vec3( -1.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  1.0f)}, 
    Vertex{glm::vec3(-x, 0.0f, z), glm::vec3(1.0f,  0.1f,  0.0f), glm::vec3( -1.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  1.0f)}, 
    Vertex{glm::vec3(-x,    y, z), glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3( -1.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  0.0f)}, 
    Vertex{glm::vec3(-x,    y,-z), glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3( -1.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  0.0f)}, 

    Vertex{glm::vec3( x, 0.0f,  z), glm::vec3(0.1f,  1.0f,  0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  1.0f)}, 
    Vertex{glm::vec3( x, 0.0f, -z), glm::vec3(0.1f,  1.0f,  0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  1.0f)},
    Vertex{glm::vec3( x,    y, -z), glm::vec3(0.0f,  0.1f,  1.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  0.0f)},
    Vertex{glm::vec3( x,    y,  z), glm::vec3(0.0f,  0.1f,  1.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  0.0f)}, 
};

Vertex vertices_bookshelf_side[] = {
    Vertex{glm::vec3(-x, 0.0f,  z), glm::vec3(1.0f,  0.1f,  0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2( 0.0f,  1.0f)},
    Vertex{glm::vec3( x, 0.0f,  z), glm::vec3(0.1f,  1.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2( 1.0f,  1.0f)},
    Vertex{glm::vec3( x,    y,  z), glm::vec3(0.0f,  0.1f,  1.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2( 1.0f,  0.0f)}, 
    Vertex{glm::vec3(-x,    y,  z), glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2( 0.0f,  0.0f)},   

    Vertex{glm::vec3(-x, 0.0f, -z), glm::vec3(1.0f,  0.1f,  0.0f), glm::vec3( 0.0f,  0.0f,  -1.0f), glm::vec2( 1.0f,  1.0f)},
    Vertex{glm::vec3( x, 0.0f, -z), glm::vec3(0.1f,  1.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  -1.0f), glm::vec2( 0.0f,  1.0f)},
    Vertex{glm::vec3( x,    y, -z), glm::vec3(0.0f,  0.1f,  1.0f), glm::vec3( 0.0f,  0.0f,  -1.0f), glm::vec2( 0.0f,  0.0f)}, 
    Vertex{glm::vec3(-x,    y, -z), glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3( 0.0f,  0.0f,  -1.0f), glm::vec2( 1.0f,  0.0f)},
};

Vertex vertices_bookshelf_top[] = {
    Vertex{glm::vec3(-x, y,  z), glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2( 0.0f,  1.0f)}, 
    Vertex{glm::vec3( x, y,  z), glm::vec3(0.0f,  0.1f,  1.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2( 1.0f,  1.0f)}, 
    Vertex{glm::vec3( x, y, -z), glm::vec3(0.0f,  0.1f,  1.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2( 1.0f,  0.0f)}, 
    Vertex{glm::vec3(-x, y, -z), glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2( 0.0f,  0.0f)},
};


GLuint indices_bookshelf[] = {
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
};


//      LIGHT
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




void Set_version(int major, int minor){
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}


int main(){
    glfwInit();
    Set_version(3, 3);

    GLFWwindow *window = glfwCreateWindow(w, h, "OpenGL", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, w, h);



    //  SHADERS
    Shader shader_program("resource/shaders/default.vert", "resource/shaders/default.frag");
    Shader light_shader("resource/shaders/light.vert", "resource/shaders/light.frag");



    //  OBJECTS

    //      FLOOR
    Texture texture_floor[]{
        Texture ("resource/textures/plank.jpg", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture ("resource/textures/plank.jpg", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
    };
    Mesh floor = create_object(vertices_floor, indices_floor, texture_floor);


    //      WALL
    Texture texture_wall[] = { Texture ("resource/textures/wall.jpg", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE) };
    Mesh wall = create_object(vertices_wall, indices_wall, texture_wall);


    //      BOOKSHELF
    Texture texture_bookshelf[] = { Texture ("resource/textures/book.jpg", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE) };
    Mesh bookshelf = create_object(vertices_bookshelf, indices_bookshelf, texture_bookshelf);

    Texture texture_bookshelf_top[] = { Texture ("resource/textures/book_top.jpg", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE) };
    Mesh bookshelf_top = create_object(vertices_bookshelf_top, indices_bookshelf, texture_bookshelf_top);

    Texture texture_bookshelf_side[] = { Texture ("resource/textures/book_side.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE) };
    Mesh bookshelf_side = create_object(vertices_bookshelf_side, indices_bookshelf, texture_bookshelf_side);


    //      RUG
    Texture texture_rug[] = { Texture ("resource/textures/rug.jpg", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE) };
    Mesh rug = create_object(vertices_rug, indices_bookshelf, texture_rug);

    Texture texture_rug_top[] = { Texture ("resource/textures/rug_top.jpg", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE) };
    Mesh rug_top = create_object(vertices_rug_top, indices_bookshelf, texture_rug_top);



    //      IMPORTED MODEL
    std::vector<Texture> modelTextures{ Texture("resource/textures/planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE) };
    Mesh girl = LoadOBJ("resource/models/girl.obj");
    girl.textures = modelTextures;


    //      LIGHT
    std::vector <Vertex> light_verts(light_vertices, light_vertices + sizeof(light_vertices) / sizeof(Vertex));
    std::vector <GLuint> light_inds(light_indices, light_indices + sizeof(light_indices) / sizeof(GLuint));
    Mesh light(light_verts, light_inds);





    //  OBJECTS ATTRIBUTES

    //      IMPORTED MODEL
    glm::vec3 girl_pos = glm::vec3(0.8f, 0.0f, -0.5f);
    glm::mat4 girl_model = glm::mat4(1.0f);
    girl_model = glm::translate(girl_model, girl_pos);


    //      FLOOR
    glm::vec3 floor_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 floor_model = glm::mat4(1.0f);
    floor_model = glm::translate(floor_model, floor_pos);
    

    //      WALL
    glm::vec3 wall_pos = glm::vec3(0.0f, 1.0f, -1.0f);
    glm::mat4 wall_model = glm::mat4(1.0f);
    wall_model = glm::translate(wall_model, wall_pos);


    //      BOOKSHELF
    glm::vec3 bookshelf_pos = glm::vec3(-0.8f, 0.001f, 0.2f);
    glm::mat4 bookshelf_model = glm::mat4(1.0f);
    bookshelf_model = glm::translate(bookshelf_model, bookshelf_pos);


     //      RUG
    glm::vec3 rug_pos = glm::vec3(-0.2f, 0.001f, 0.2f);
    glm::mat4 rug_model = glm::mat4(1.0f);
    rug_model = glm::translate(rug_model, rug_pos);


    //      LIGHT
    glm::vec4 light_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 light_pos = glm::vec3(0.8f, 0.8f, 0.8f);
    glm::mat4 light_model = glm::mat4(1.0f);
    light_model = glm::translate(light_model, light_pos);



    // Shaders uniforms 

    //      LIGHT
    light_shader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(light_shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(light_model));
    glUniform4f(glGetUniformLocation(light_shader.ID, "light_color"), light_color.x, light_color.y, light_color.z, light_color.w);


    //      PROGRAM
    shader_program.Activate();
    glUniform4f(glGetUniformLocation(shader_program.ID, "light_color"), light_color.x, light_color.y, light_color.z, light_color.w);
    glUniform3f(glGetUniformLocation(shader_program.ID, "light_pos"), light_pos.x, light_pos.y, light_pos.z);


    // ENABLES DEPTH
    glEnable(GL_DEPTH_TEST);


    // Camera attributes
    Camera camera (w, h, glm::vec3(0.0f, 1.0f, 2.0f));


    // "World" colors
    float r = 2, g = 50, b = 77, a = 1, rgb_code = 256;


    float last_time = glfwGetTime();

    while (!glfwWindowShouldClose(window)){
        glClearColor(r/rgb_code, g/rgb_code, b/rgb_code, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // resets camera's position if user goes too far
        if (camera.Position.x > 10.0f || camera.Position.y > 10.0f || camera.Position.z > 10.0f){
            camera.Position = glm::vec3(0.0f, 1.0f, 2.0f);
        }


        // Just to fix FPS speeding up issues
        float current_time = glfwGetTime();
        float delta_time = current_time - last_time;
        last_time = current_time;

        
        // Controllable camera!
        camera.Inputs(window, delta_time);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);


        shader_program.Activate();

        bookshelf.Draw_mesh(bookshelf_model, shader_program, camera);
        bookshelf_top.Draw_mesh(bookshelf_model, shader_program, camera);
        bookshelf_side.Draw_mesh(bookshelf_model, shader_program, camera);
        
        girl.Draw_mesh(girl_model, shader_program, camera);
        floor.Draw_mesh(floor_model, shader_program, camera);
        wall.Draw_mesh(wall_model, shader_program, camera);

        rug.Draw_mesh(rug_model, shader_program, camera);
        rug_top.Draw_mesh(rug_model, shader_program, camera);

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