#include "Mesh.h"
#include "tiny_obj_loader.h"
#include "table.h"
#include "pokeball.h"
#include "desk.h"
#include "Model.h"

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


void bookshelf_draw(Mesh bookshelf, Mesh bookshelf_top, Mesh bookshelf_side, glm::mat4 bookshelf_model, Shader shader_program, Camera camera){
    bookshelf.Draw_mesh(bookshelf_model, shader_program, camera);
    bookshelf_top.Draw_mesh(bookshelf_model, shader_program, camera);
    bookshelf_side.Draw_mesh(bookshelf_model, shader_program, camera);
}

// Window res
const unsigned int w = 1080;
const unsigned int h = 720;


// OBJECTS VERTICES AND INDICES

//      FLOOR
Vertex vertices_floor[] = {
    //                      COORDS                          NORMALS                         COLORS               UV (text pos)
    Vertex{glm::vec3(-4.0f,  0.0f,  5.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  0.0f)},
    Vertex{glm::vec3(-4.0f,  0.0f, -5.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  1.0f)},
    Vertex{glm::vec3( 4.0f,  0.0f, -5.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  1.0f)},
    Vertex{glm::vec3( 4.0f,  0.0f,  5.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  0.0f)}
};


//      Walls
float x_wls = 4.0, y_wls = 2, z_wls = 5.0;
Vertex vertices_room[] = {
    // left
    Vertex{glm::vec3(-x_wls,    0.0f,-z_wls), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  1.0f)}, 
    Vertex{glm::vec3(-x_wls,    0.0f, z_wls), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  1.0f)}, 
    Vertex{glm::vec3(-x_wls,   y_wls, z_wls), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  0.0f)}, 
    Vertex{glm::vec3(-x_wls,   y_wls,-z_wls), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  0.0f)}, 

    // right
    Vertex{glm::vec3( x_wls,    0.0f,  z_wls), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  1.0f)}, 
    Vertex{glm::vec3( x_wls,    0.0f, -z_wls), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  1.0f)},
    Vertex{glm::vec3( x_wls,   y_wls, -z_wls), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  0.0f)},
    Vertex{glm::vec3( x_wls,   y_wls,  z_wls), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  0.0f)}, 

    // front
    Vertex{glm::vec3(-x_wls,    0.0f,  z_wls), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  1.0f)},
    Vertex{glm::vec3( x_wls,    0.0f,  z_wls), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  1.0f)},
    Vertex{glm::vec3( x_wls,   y_wls,  z_wls), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  0.0f)}, 
    Vertex{glm::vec3(-x_wls,   y_wls,  z_wls), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  0.0f)},   

    // back
    Vertex{glm::vec3(-x_wls,    0.0f, -z_wls), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  1.0f)},
    Vertex{glm::vec3( x_wls,    0.0f, -z_wls), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  1.0f)},
    Vertex{glm::vec3( x_wls,   y_wls, -z_wls), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  0.0f)}, 
    Vertex{glm::vec3(-x_wls,   y_wls, -z_wls), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  0.0f)},
};


//      RUG 
float rx = 0.2, ry = 0.02, rz = 0.5;
Vertex vertices_rug[] = {
    Vertex{glm::vec3(-rx, 0.0f, -rz), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  1.0f)}, 
    Vertex{glm::vec3(-rx, 0.0f,  rz), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  1.0f)}, 
    Vertex{glm::vec3(-rx,   ry,  rz), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  0.0f)}, 
    Vertex{glm::vec3(-rx,   ry, -rz), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  0.0f)}, 

    Vertex{glm::vec3( rx, 0.0f,  rz), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  1.0f)}, 
    Vertex{glm::vec3( rx, 0.0f, -rz), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  1.0f)},
    Vertex{glm::vec3( rx,   ry, -rz), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  0.0f)},
    Vertex{glm::vec3( rx,   ry,  rz), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  0.0f)}, 

    Vertex{glm::vec3(-rx, 0.0f,  rz), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  1.0f)},
    Vertex{glm::vec3( rx, 0.0f,  rz), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  1.0f)},
    Vertex{glm::vec3( rx,   ry,  rz), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  0.0f)}, 
    Vertex{glm::vec3(-rx,   ry,  rz), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  0.0f)},   

    Vertex{glm::vec3(-rx, 0.0f, -rz), glm::vec3(0.0f,  0.0f,  -1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  1.0f)},
    Vertex{glm::vec3( rx, 0.0f, -rz), glm::vec3(0.0f,  0.0f,  -1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  1.0f)},
    Vertex{glm::vec3( rx,   ry, -rz), glm::vec3(0.0f,  0.0f,  -1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  0.0f)}, 
    Vertex{glm::vec3(-rx,   ry, -rz), glm::vec3(0.0f,  0.0f,  -1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  0.0f)},
};

Vertex vertices_rug_top[] = {
    Vertex{glm::vec3(-rx,   ry,  rz), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  1.0f)}, 
    Vertex{glm::vec3( rx,   ry,  rz), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  1.0f)}, 
    Vertex{glm::vec3( rx,   ry, -rz), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  0.0f)}, 
    Vertex{glm::vec3(-rx,   ry, -rz), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  0.0f)},
};


//      BOOKSHELF
float x = 0.25, y = 1.6, z = 0.5;
Vertex vertices_bookshelf[] = {
    // left
    Vertex{glm::vec3(-x, 0.0f,-z), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  1.0f)}, 
    Vertex{glm::vec3(-x, 0.0f, z), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  1.0f)}, 
    Vertex{glm::vec3(-x,    y, z), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  0.0f)}, 
    Vertex{glm::vec3(-x,    y,-z), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  0.0f)}, 

    // right
    Vertex{glm::vec3( x, 0.0f,  z), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  1.0f)}, 
    Vertex{glm::vec3( x, 0.0f, -z), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  1.0f)},
    Vertex{glm::vec3( x,    y, -z), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  0.0f)},
    Vertex{glm::vec3( x,    y,  z), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  0.0f)}, 
};

Vertex vertices_bookshelf_side[] = {
    // front
    Vertex{glm::vec3(-x, 0.0f,  z), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  1.0f)},
    Vertex{glm::vec3( x, 0.0f,  z), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  1.0f)},
    Vertex{glm::vec3( x,    y,  z), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  0.0f)}, 
    Vertex{glm::vec3(-x,    y,  z), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  0.0f)},   

    // back
    Vertex{glm::vec3(-x, 0.0f, -z), glm::vec3(0.0f,  0.0f,  -1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  1.0f)},
    Vertex{glm::vec3( x, 0.0f, -z), glm::vec3(0.0f,  0.0f,  -1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  1.0f)},
    Vertex{glm::vec3( x,    y, -z), glm::vec3(0.0f,  0.0f,  -1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  0.0f)}, 
    Vertex{glm::vec3(-x,    y, -z), glm::vec3(0.0f,  0.0f,  -1.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  0.0f)},
};

Vertex vertices_bookshelf_top[] = {
    Vertex{glm::vec3(-x, y,  z), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  1.0f)}, 
    Vertex{glm::vec3( x, y,  z), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  1.0f)}, 
    Vertex{glm::vec3( x, y, -z), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  0.0f)}, 
    Vertex{glm::vec3(-x, y, -z), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  0.0f)},
};

GLfloat vertices[] = {
    // VERTICES COORDS            COLOR              UV (text pos)            NORMALS 
    -0.4f, -0.4f, 0.4f,     1.0f, 0.1f, 0.0f,       0.0f, 1.0f,         0.0f, 0.0f, 1.0f, // front bottom left (0)  
    0.4f,  -0.4f, 0.4f,      0.1f, 1.0f, 0.0f,       1.0f, 1.0f,         0.0f, 0.0f, 1.0f, // front bottom right (1)
    0.4f,   0.8f, 0.4f,       0.0f, 0.1f, 1.0f,       1.0f, 0.0f,         0.0f, 0.0f, 1.0f, // front top right (2)
    -0.4f,  0.8f, 0.4f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f, 0.0f, 1.0f, // front top left (3)

    -0.4f, -0.4f, -0.4f,     1.0f, 0.1f, 0.0f,       1.0f, 1.0f,        0.0f, 0.0f, -1.0f, // back bottom left (4)
    0.4f,  -0.4f, -0.4f,      0.1f, 1.0f, 0.0f,       0.0f, 1.0f,        0.0f, 0.0f, -1.0f, // back bottom right (5)
    0.4f,   0.8f, -0.4f,       0.0f, 0.1f, 1.0f,       0.0f, 0.0f,        0.0f, 0.0f, -1.0f, // back top right (6)
    -0.4f,  0.8f, -0.4f,      1.0f, 1.0f, 1.0f,       1.0f, 0.0f,        0.0f, 0.0f, -1.0f, // back top left (7)
    
    -0.4f, -0.4f, -0.4f,     1.0f, 0.1f, 0.0f,       1.0f, 1.0f,        -1.0f, 0.0f, 0.0f, // left bottom left (8)
    -0.4f, -0.4f, 0.4f,     1.0f, 0.1f, 0.0f,       0.0f, 1.0f,         -1.0f, 0.0f, 0.0f, // left bottom left (9)
    -0.4f,  0.8f, 0.4f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         -1.0f, 0.0f, 0.0f, // left top left (10)
    -0.4f,  0.8f, -0.4f,      1.0f, 1.0f, 1.0f,       1.0f, 0.0f,        -1.0f, 0.0f, 0.0f, // left top left (11)

    0.4f, -0.4f, 0.4f,      0.1f, 1.0f, 0.0f,       1.0f, 1.0f,         1.0f, 0.0f, 0.0f, // right bottom right (12)
    0.4f, -0.4f, -0.4f,      0.1f, 1.0f, 0.0f,       0.0f, 1.0f,        1.0f, 0.0f, 0.0f, // right bottom right (13)
    0.4f,  0.8f, -0.4f,       0.0f, 0.1f, 1.0f,       0.0f, 0.0f,        1.0f, 0.0f, 0.0f, // right top right (14)
    0.4f,  0.8f, 0.4f,       0.0f, 0.1f, 1.0f,       1.0f, 0.0f,         1.0f, 0.0f, 0.0f, // right top right (15)

    -0.4f, -0.4f, 0.4f,     1.0f, 0.1f, 0.0f,       0.0f, 1.0f,         0.0f, -1.0f, 0.0f, // top bottom left (16)
    0.4f,  -0.4f, 0.4f,      0.1f, 1.0f, 0.0f,       1.0f, 1.0f,         0.0f, -1.0f, 0.0f, // top bottom right (17)
    0.4f,  -0.4f, -0.4f,      0.1f, 1.0f, 0.0f,       1.0f, 0.0f,        0.0f, -1.0f, 0.0f, // top bottom right (18)
    -0.4f, -0.4f, -0.4f,     1.0f, 0.1f, 0.0f,       0.0f, 0.0f,        0.0f, -1.0f, 0.0f, // top bottom left (19)

    -0.4f, 0.8f, 0.4f,      1.0f, 1.0f, 1.0f,       0.0f, 1.0f,         0.0f, 1.0f, 0.0f, // bottom top left (20)
    0.4f,  0.8f, 0.4f,       0.0f, 0.1f, 1.0f,       1.0f, 1.0f,         0.0f, 1.0f, 0.0f, // bottom top right (21)
    0.4f,  0.8f, -0.4f,       0.0f, 0.1f, 1.0f,       1.0f, 0.0f,        0.0f, 1.0f, 0.0f, // bottom top right (22)
    -0.4f, 0.8f, -0.4f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,        0.0f, 1.0f, 0.0f, // bottom top left (23)
};


GLuint indices[] = {
    0, 1, 2,
    2, 3, 0,

    12, 13, 14,
    14, 15, 12,

    4, 5, 6,
    6, 7, 4,

    8, 11, 10,
    10, 9, 8, 

    16, 17, 18,
    18, 19, 16,

    20, 21, 22,
    22, 23, 20
};

//Cubo usado de base para a escrivaninha
Vertex verticesCubo[] = {
    //Frente
    Vertex{glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(0,0,1), glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3( 0.5f,-0.5f, 0.5f), glm::vec3(0,0,1), glm::vec3(1,1,1), glm::vec2(1,0)},
    Vertex{glm::vec3( 0.5f, 0.5f, 0.5f), glm::vec3(0,0,1), glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0,0,1), glm::vec3(1,1,1), glm::vec2(0,1)},

    //Trás
    Vertex{glm::vec3( 0.5f,-0.5f,-0.5f), glm::vec3(0,0,-1), glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0,0,-1), glm::vec3(1,1,1), glm::vec2(1,0)},
    Vertex{glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(0,0,-1), glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3( 0.5f, 0.5f,-0.5f), glm::vec3(0,0,-1), glm::vec3(1,1,1), glm::vec2(0,1)},

    //Esquerda
    Vertex{glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(-1,0,0), glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(-1,0,0), glm::vec3(1,1,1), glm::vec2(1,0)},
    Vertex{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-1,0,0), glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(-1,0,0), glm::vec3(1,1,1), glm::vec2(0,1)},

    //Direita
    Vertex{glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(1,0,0), glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(1,0,0), glm::vec3(1,1,1), glm::vec2(1,0)},
    Vertex{glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(1,0,0), glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1,0,0), glm::vec3(1,1,1), glm::vec2(0,1)},

    //Topo
    Vertex{glm::vec3(-0.5f,0.5f, 0.5f), glm::vec3(0,1,0), glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3( 0.5f,0.5f, 0.5f), glm::vec3(0,1,0), glm::vec3(1,1,1), glm::vec2(1,0)},
    Vertex{glm::vec3( 0.5f,0.5f,-0.5f), glm::vec3(0,1,0), glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(-0.5f,0.5f,-0.5f), glm::vec3(0,1,0), glm::vec3(1,1,1), glm::vec2(0,1)},

    //Base
    Vertex{glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0,-1,0), glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3( 0.5f,-0.5f,-0.5f), glm::vec3(0,-1,0), glm::vec3(1,1,1), glm::vec2(1,0)},
    Vertex{glm::vec3( 0.5f,-0.5f, 0.5f), glm::vec3(0,-1,0), glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(0,-1,0), glm::vec3(1,1,1), glm::vec2(0,1)},
};

GLuint indicesCubo[] = {

    0,1,2, 2,3,0,       // frente
    4,5,6, 6,7,4,       // trás
    8,9,10, 10,11,8,    // esquerda
    12,13,14, 14,15,12, // direita
    16,17,18, 18,19,16, // topo
    20,21,22, 22,23,20  // base
};


//Tronco de pirâmide usado para modelar partes da escrivaninha
GLuint indicesTroncoP[] = {
	0,1,2,	2,3,0,
	4,5,6,	6,7,4,
	8,9,10,	10,11,8,
	12,13,14,	14,15,12,
	16,17,18,	18,19,16,
    20,21,22,   22,23,20
};

Vertex troncoPiramide[] = {
    // PLACA DA FRENTE
    Vertex{glm::vec3(0.015f, 0.015f,  0.01f), glm::vec3(0,0,1), glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3(-0.015f, 0.015f,  0.01f), glm::vec3(0,0,1), glm::vec3(1,1,1), glm::vec2(0,1)},
    Vertex{glm::vec3(-0.015f, -0.015f,  0.01f), glm::vec3(0,0,1), glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(0.015f, -0.015f,  0.01f), glm::vec3(0,0,1), glm::vec3(1,1,1), glm::vec2(1,0)},

    //Placa tras
    Vertex{glm::vec3(0.0225f, 0.0225f,  0.0f), glm::vec3(0,0,-1), glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3(-0.0225f, 0.0225f,  0.0f), glm::vec3(0,0,-1), glm::vec3(1,1,1), glm::vec2(0,1)},
    Vertex{glm::vec3(-0.0225f, -0.0225f,  0.0f), glm::vec3(0,0,-1), glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(0.0225f, -0.0225f,  0.0f), glm::vec3(0,0,-1), glm::vec3(1,1,1), glm::vec2(1,0)},

    // ESQUERDA
    Vertex{glm::vec3(-0.015f, -0.015f, 0.01f), glm::vec3(-1,0,0), glm::vec3(1,1,1), glm::vec2(1,0)},
    Vertex{glm::vec3(-0.015f, 0.015f, 0.01f), glm::vec3(-1,0,0), glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3(-0.0225f, 0.0225f, 0.0f), glm::vec3(-1,0,0), glm::vec3(1,1,1), glm::vec2(0,1)},
    Vertex{glm::vec3(-0.0225f, -0.0225f, 0.0f), glm::vec3(-1,0,0), glm::vec3(1,1,1), glm::vec2(1,1)},
 	
    // DIREITA
    Vertex{glm::vec3(0.0151f, 0.015f, 0.01f), glm::vec3(1,0,0), glm::vec3(1,1,1), glm::vec2(1,0)},
    Vertex{glm::vec3(0.015f, -0.015f, 0.01f), glm::vec3(1,0,0), glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3(0.0225f, -0.0225f, 0.0f), glm::vec3(1,0,0), glm::vec3(1,1,1), glm::vec2(0,1)},
    Vertex{glm::vec3(0.0225f, 0.0225f, 0.0f), glm::vec3(1,0,0), glm::vec3(1,1,1), glm::vec2(1,1)},
    
	// BAIXO
    Vertex{glm::vec3(0.015f, -0.015f, 0.01f), glm::vec3(0,-1,0), glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3(-0.015f, -0.015f, 0.01f), glm::vec3(0,-1,0), glm::vec3(1,1,1), glm::vec2(0,1)},
    Vertex{glm::vec3(-0.0225f, -0.0225f, 0.0f), glm::vec3(0,-1,0), glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(0.0225f, -0.0225f, 0.0f), glm::vec3(0,-1,0), glm::vec3(1,1,1), glm::vec2(1,0)},
    
	// CIMA
    Vertex{glm::vec3(-0.015f, 0.015f, 0.01f), glm::vec3(0,1,0), glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3(0.015f, 0.015f, 0.01f), glm::vec3(0,1,0), glm::vec3(1,1,1), glm::vec2(0,1)},
    Vertex{glm::vec3(0.0225f, 0.0225f, 0.0f), glm::vec3(0,1,0), glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(-0.0225f, 0.0225f, 0.0f), glm::vec3(0,1,0), glm::vec3(1,1,1), glm::vec2(1,0)}
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
    Shader pokeballShader("resource/shaders/pokeball.vert", "resource/shaders/pokeball.frag");



    //  OBJECTS

    //      FLOOR
    Texture texture_floor[]{
        Texture ("resource/textures/plank.jpg", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture ("resource/textures/plank.jpg", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
    };
    Mesh floor = create_object(vertices_floor, indices, texture_floor);

    //      WALL
    Texture texture_wall[] = { Texture ("resource/textures/wall.jpg", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE) };
    Mesh wall = create_object(vertices_room, indices, texture_wall);

    //      BOOKSHELF
    Texture texture_bookshelf[] = { Texture ("resource/textures/book.jpg", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE) };
    Mesh bookshelf = create_object(vertices_bookshelf, indices, texture_bookshelf);

    Texture texture_bookshelf_top[] = { Texture ("resource/textures/book_top.jpg", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE) };
    Mesh bookshelf_top = create_object(vertices_bookshelf_top, indices, texture_bookshelf_top);

    Texture texture_bookshelf_side[] = { Texture ("resource/textures/book_side.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE) };
    Mesh bookshelf_side = create_object(vertices_bookshelf_side, indices, texture_bookshelf_side);

    //      RUG
    Texture texture_rug[] = { Texture ("resource/textures/rug.jpg", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE) };
    Mesh rug = create_object(vertices_rug, indices, texture_rug);

    Texture texture_rug_top[] = { Texture ("resource/textures/rug_top.jpg", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE) };
    Mesh rug_top = create_object(vertices_rug_top, indices, texture_rug_top);



    //      DESK
    Texture deskTex("resource/textures/desk.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE);



    //      WIDOW
    Texture windowTexture("resource/textures/molduraTexture.jpeg", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture glassTexture("resource/textures/glassTexture.jpeg", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE);

    //      DOOR
    Texture doorTexture("resource/textures/doorTexture.jpeg", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE);

    //      POKEBALL
    float x = 2.5, y = 0.8, z = -2.0;
    Pokeball pokeballCenter(glm::vec3(x +  0.0f,   y + -0.35f, z + 0.0f), 0.06f);
    Pokeball pokeballLeft(glm::vec3(  x + -0.45f,  y + -0.35f, z + 0.0f), 0.060f);
    Pokeball pokeballRight(glm::vec3( x +  0.45f,  y + -0.35f, z + 0.0f), 0.060f);

    //      TABLE
    Texture branco("resource/textures/white.jpg", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture verde("resource/textures/green.jpg", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE);

    std::vector<Vertex> verts;
    std::vector<GLuint> inds(indices, indices + sizeof(indices) / sizeof(GLuint));
    for (int i = 0; i < 24; i++){
        Vertex v;
        v.position = glm::vec3(vertices[i * 11 + 0], vertices[i * 11 + 1], vertices[i * 11 + 2]);
        v.color = glm::vec3(vertices[i * 11 + 3], vertices[i * 11 + 4], vertices[i * 11 + 5]);
        v.texUV = glm::vec2(vertices[i * 11 + 6], vertices[i * 11 + 7]);
        v.normal = glm::vec3(vertices[i * 11 + 8], vertices[i * 11 + 9], vertices[i * 11 + 10]);
        verts.push_back(v);
    }
    
    std::vector<Texture> texBranco{ branco };
    std::vector<Texture> texVerde{ verde };

    Mesh meshBranco(verts, inds, texBranco);
    Mesh meshVerde(verts, inds, texVerde);
    Table table(glm::vec3(x, y, z), meshBranco, meshVerde);

    

    //      LIGHT
    std::vector <Vertex> light_verts(light_vertices, light_vertices + sizeof(light_vertices) / sizeof(Vertex));
    std::vector <GLuint> light_inds(light_indices, light_indices + sizeof(light_indices) / sizeof(GLuint));
    Mesh light(light_verts, light_inds);



    //  OBJECTS ATTRIBUTES

    //      FLOOR
    glm::vec3 floor_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 floor_model = glm::mat4(1.0f);
    floor_model = glm::translate(floor_model, floor_pos);
    

    //      WALL
    glm::vec3 wall_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 wall_model = glm::mat4(1.0f);
    wall_model = glm::translate(wall_model, wall_pos);


    //      BOOKSHELVES
    glm::vec3 bookshelf_pos_1 = glm::vec3(-3.45f, 0.001f,  1.0f);
    glm::vec3 bookshelf_pos_2 = glm::vec3(-2.4f,  0.001f,  1.0f);
    glm::vec3 bookshelf_pos_3 = glm::vec3(-1.35f, 0.001f,  1.0f);
    glm::vec3 bookshelf_pos_4 = glm::vec3( 3.45f, 0.001f,  1.0f);
    glm::vec3 bookshelf_pos_5 = glm::vec3( 2.4f,  0.001f,  1.0f);
    glm::vec3 bookshelf_pos_6 = glm::vec3( 1.35f, 0.001f,  1.0f);
    glm::vec3 bookshelf_pos_7 = glm::vec3(-3.45f, 0.001f, -4.74f);
    glm::vec3 bookshelf_pos_8 = glm::vec3( 3.45f, 0.001f, -4.74f);

    glm::mat4 bookshelf_model_1 = glm::mat4(1.0f);
    bookshelf_model_1 = glm::translate(bookshelf_model_1, bookshelf_pos_1);
    bookshelf_model_1 = glm::rotate(bookshelf_model_1, 1.57f, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 bookshelf_model_2 = glm::mat4(1.0f);
    bookshelf_model_2 = glm::translate(bookshelf_model_2, bookshelf_pos_2);
    bookshelf_model_2 = glm::rotate(bookshelf_model_2, 1.57f, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 bookshelf_model_3 = glm::mat4(1.0f);
    bookshelf_model_3 = glm::translate(bookshelf_model_3, bookshelf_pos_3);
    bookshelf_model_3 = glm::rotate(bookshelf_model_3, 1.57f, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 bookshelf_model_4 = glm::mat4(1.0f);
    bookshelf_model_4 = glm::translate(bookshelf_model_4, bookshelf_pos_4);
    bookshelf_model_4 = glm::rotate(bookshelf_model_4, 1.57f, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 bookshelf_model_5 = glm::mat4(1.0f);
    bookshelf_model_5 = glm::translate(bookshelf_model_5, bookshelf_pos_5);
    bookshelf_model_5 = glm::rotate(bookshelf_model_5, 1.57f, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 bookshelf_model_6 = glm::mat4(1.0f);
    bookshelf_model_6 = glm::translate(bookshelf_model_6, bookshelf_pos_6);
    bookshelf_model_6 = glm::rotate(bookshelf_model_6, 1.57f, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 bookshelf_model_7 = glm::mat4(1.0f);
    bookshelf_model_7 = glm::translate(bookshelf_model_7, bookshelf_pos_7);
    bookshelf_model_7 = glm::rotate(bookshelf_model_7, 1.57f, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 bookshelf_model_8 = glm::mat4(1.0f);
    bookshelf_model_8 = glm::translate(bookshelf_model_8, bookshelf_pos_8);
    bookshelf_model_8 = glm::rotate(bookshelf_model_8, 1.57f, glm::vec3(0.0f, 1.0f, 0.0f));

    //      RUG
    glm::vec3 rug_pos = glm::vec3(0.0f, 0.001f, 4.8f);
    glm::mat4 rug_model = glm::mat4(1.0f);
    rug_model = glm::translate(rug_model, rug_pos);
    rug_model = glm::rotate(rug_model, 1.57f, glm::vec3(0.0f, 1.0f, 0.0f));



    //      POKEDEX
    glm::vec3 poked_pos1 = glm::vec3(-0.6f, 0.75f, -4.6f);
    glm::vec3 poked_pos2 = glm::vec3(-1.3f, 0.75f, -4.6f);
    glm::vec3 poked_rotat1 = glm::vec3(0.0f, -45.0f, 90.0f);
    glm::vec3 poked_rotat2 = glm::vec3(0.0f, -90.0f, 90.0f);
    glm::vec3 poked_sca = glm::vec3(0.2f, 0.2f, 0.2f);
    Model pokedex1("resource/models/pokedex/SegundaOpcao/scene.gltf", poked_pos1, poked_rotat1, poked_sca);
    Model pokedex2("resource/models/pokedex/SegundaOpcao/scene.gltf", poked_pos2, poked_rotat2, poked_sca);

    //      PLANTA
    glm::vec3 plant_pos1 = glm::vec3( 1.4f, 0.0f, -4.5f);
    glm::vec3 plant_pos2 = glm::vec3(-3.5f, 0.0f, 4.5f);
    glm::vec3 plant_pos3 = glm::vec3( 3.5f, 0.0f, 4.5f);
    Model planta1("resource/models/plant/scene.gltf", plant_pos1, glm::vec3(0), glm::vec3(0.192));
    Model planta2("resource/models/plant/scene.gltf", plant_pos2, glm::vec3(0), glm::vec3(0.256));
    Model planta3("resource/models/plant/scene.gltf", plant_pos3, glm::vec3(0), glm::vec3(0.256));

    //      HEALER
    Model healer("resource/models/healer/scene.gltf", glm::vec3(-3.35f, 0.0f, -2.0f), glm::vec3(0), glm::vec3(0.0256));

    //      ESCRIVANINHA
    std::vector<Texture> mesa{deskTex};
    std::vector <Vertex> vertsCubo(verticesCubo, verticesCubo + sizeof(verticesCubo)/sizeof(Vertex));
    std::vector <GLuint> indsCubo(indicesCubo, indicesCubo + sizeof(indicesCubo) /  sizeof(GLuint));
    Mesh cubo(vertsCubo, indsCubo, mesa);

    std::vector <Vertex> vertsTroncoP(troncoPiramide, troncoPiramide + sizeof(troncoPiramide)/sizeof(Vertex));
    std::vector <GLuint> indsTroncoP(indicesTroncoP, indicesTroncoP + sizeof(indicesTroncoP)/sizeof(GLuint));
    Mesh troncoPiramideM(vertsTroncoP, indsTroncoP, mesa);

    Desk desk(&cubo, &troncoPiramideM, glm::vec3(-1.0f, 0.37f, -4.6f));



    //      PORTA
    Mesh door = Mesh::GenerateCube(1.0f);
    door.textures = { doorTexture };
    glm::vec3 door_pos = glm::vec3(0.0f, 0.9f, 5.0f);
    glm::mat4 door_model = glm::mat4(1.0f);

    door_model = glm::translate(door_model, door_pos);

    door_model = glm::scale(door_model, glm::vec3(0.9f, 2.0f, 0.05f));

    //      JANELA
    std::vector<Texture> texWindow{ windowTexture };
    Mesh windowFrame = Mesh::GenerateCube(1.0f);
    windowFrame.textures = { windowTexture };
    Mesh windowGlass = Mesh::GenerateCube(1.0f);
    windowGlass.textures = { glassTexture }; 
    glm::vec3 window_pos = glm::vec3(1.4f, 1.3f, -4.98f);

    glm::mat4 windowFrame_model = glm::mat4(1.0f);
    windowFrame_model = glm::translate(windowFrame_model, window_pos);
    windowFrame_model = glm::scale(windowFrame_model, glm::vec3(0.8f, 1.0f, 0.02f));
    glm::mat4 windowGlass_model = glm::mat4(1.0f);
    windowGlass_model = glm::translate(windowGlass_model, window_pos + glm::vec3(0.0f, 0.0f, 0.01f));
    windowGlass_model = glm::scale(windowGlass_model, glm::vec3(0.6f, 0.8f, 0.01f));






    
    // Light uniforms
    glm::vec3 light_pos[] = {
        glm::vec3(0.0f, 2.0f, 0.0f),
        glm::vec3(-3.0f, 2.0f, -3.0f),
        glm::vec3(3.0f,  2.0f, -3.0f),
        glm::vec3(3.0f,  2.0f,  3.0f),
        glm::vec3(-3.0f, 2.0f,  3.0f)
    };

    shader_program.Activate();

    for (int i = 0; i < 5; i++){
        std::string pos_name = "light_pos["+std::to_string(i) + "]";
        std::string color_name = "light_color["+std::to_string(i) + "]";

        glUniform3fv(glGetUniformLocation(shader_program.ID, pos_name.c_str()), 1, glm::value_ptr(light_pos[i]));
        glUniform4f(glGetUniformLocation(shader_program.ID, color_name.c_str()), 1.0f, 1.0f, 1.0f, 1.0f);
    }



    
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
        // Bookshelf
        bookshelf_draw(bookshelf, bookshelf_top, bookshelf_side, bookshelf_model_1, shader_program, camera);
        bookshelf_draw(bookshelf, bookshelf_top, bookshelf_side, bookshelf_model_2, shader_program, camera);
        bookshelf_draw(bookshelf, bookshelf_top, bookshelf_side, bookshelf_model_3, shader_program, camera);
        bookshelf_draw(bookshelf, bookshelf_top, bookshelf_side, bookshelf_model_4, shader_program, camera);
        bookshelf_draw(bookshelf, bookshelf_top, bookshelf_side, bookshelf_model_5, shader_program, camera);
        bookshelf_draw(bookshelf, bookshelf_top, bookshelf_side, bookshelf_model_6, shader_program, camera);
        bookshelf_draw(bookshelf, bookshelf_top, bookshelf_side, bookshelf_model_7, shader_program, camera);
        bookshelf_draw(bookshelf, bookshelf_top, bookshelf_side, bookshelf_model_8, shader_program, camera);
        
        // Floor & Wall
        floor.Draw_mesh(floor_model, shader_program, camera);
        wall.Draw_mesh(wall_model, shader_program, camera);

        // Tapete
        rug.Draw_mesh(rug_model, shader_program, camera);
        rug_top.Draw_mesh(rug_model, shader_program, camera);


        // Pokedex
        pokedex1.Draw(shader_program, camera);
        pokedex2.Draw(shader_program, camera);

        // Planta
        planta1.Draw(shader_program, camera);
        planta2.Draw(shader_program, camera);
        planta3.Draw(shader_program, camera);

        // Healer
        healer.Draw(shader_program,  camera);

        // Escrivaninha
        desk.Draw(shader_program, camera);
        

        // Tabela
        table.draw(shader_program, camera);

        // Janela
        windowFrame.Draw_mesh(windowFrame_model, shader_program, camera);
        windowGlass.Draw_mesh(windowGlass_model, shader_program, camera);

        // Porta
        door.Draw_mesh(door_model, shader_program, camera);

        // Pokebola
        pokeballShader.Activate();

        pokeballCenter.draw(pokeballShader,camera);
        pokeballLeft.draw(pokeballShader,camera);
        pokeballRight.draw(pokeballShader,camera);



        // Drawing multiples lights 
        light_shader.Activate();
        for (int i = 0; i < 5; i++){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, light_pos[i]);

            glUniformMatrix4fv(glGetUniformLocation(light_shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniform4f(glGetUniformLocation(light_shader.ID, "light_color"), 1.0f, 1.0f, 1.0f, 1.0f);

            light.Draw(light_shader, camera);
        }
            

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    shader_program.Delete();
    light_shader.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}