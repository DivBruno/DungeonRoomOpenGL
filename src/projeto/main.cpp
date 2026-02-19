#include "Mesh.h"
#include "tiny_obj_loader.h"
#include "table.h"
#include "pokeball.h"

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
    Vertex{glm::vec3(-5.0f,  0.0f,  5.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  0.0f)},
    Vertex{glm::vec3(-5.0f,  0.0f, -5.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 0.0f,  1.0f)},
    Vertex{glm::vec3( 5.0f,  0.0f, -5.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  1.0f)},
    Vertex{glm::vec3( 5.0f,  0.0f,  5.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec2( 1.0f,  0.0f)}
};


//      Walls
float x_wls = 5.0, y_wls = 2, z_wls = 5.0;
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
    18, 19, 16
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

    Texture branco("resource/textures/white.jpg", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture verde("resource/textures/green.jpg", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE);

    std::vector<Vertex> verts;
    std::vector<GLuint> inds(indices, indices + sizeof(indices) / sizeof(GLuint));

    for (int i = 0; i < 24; i++)
        {
        Vertex v;

        v.position = glm::vec3(
            vertices[i * 11 + 0],
            vertices[i * 11 + 1],
            vertices[i * 11 + 2]
        );

        v.color = glm::vec3(
            vertices[i * 11 + 3],
            vertices[i * 11 + 4],
            vertices[i * 11 + 5]
        );

        v.texUV = glm::vec2(    
            vertices[i * 11 + 6],
            vertices[i * 11 + 7]
        );

        v.normal = glm::vec3(
            vertices[i * 11 + 8],
            vertices[i * 11 + 9],
            vertices[i * 11 + 10]
        );

        verts.push_back(v);
    }


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



    float x = -3, y = 0, z = 0;
    Pokeball pokeballCenter(glm::vec3(x +  0.0f,   y + -0.35f, z + 0.0f), 0.06f);
    Pokeball pokeballLeft(glm::vec3(  x + -0.45f,  y + -0.35f, z + 0.0f), 0.060f);
    Pokeball pokeballRight(glm::vec3( x +  0.45f,  y + -0.35f, z + 0.0f), 0.060f);




    // ===== VETORES PARA A MESA =====
    std::vector<Texture> texBranco{ branco };
    std::vector<Texture> texVerde{ verde };




    // reutilizando o mesmo quadrado (verts e inds)
    Mesh meshBranco(verts, inds, texBranco);
    Mesh meshVerde(verts, inds, texVerde);

    // ===== CRIA TABLE =====
    Table table(
        glm::vec3(0.0f, 2.0f, 0.0f),
        meshBranco,
        meshVerde
    );




    /*
    //      IMPORTED MODEL
    std::vector<Texture> modelTextures{ Texture("resource/textures/planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE) };
    Mesh girl = LoadOBJ("resource/models/girl.obj");
    girl.textures = modelTextures;
    */


    //      LIGHT
    std::vector <Vertex> light_verts(light_vertices, light_vertices + sizeof(light_vertices) / sizeof(Vertex));
    std::vector <GLuint> light_inds(light_indices, light_indices + sizeof(light_indices) / sizeof(GLuint));
    Mesh light(light_verts, light_inds);





    //  OBJECTS ATTRIBUTES

    /*
    //      IMPORTED MODEL
    glm::vec3 girl_pos = glm::vec3(0.8f, 0.0f, -0.5f);
    glm::mat4 girl_model = glm::mat4(1.0f);
    girl_model = glm::translate(girl_model, girl_pos);
    */

    //      FLOOR
    glm::vec3 floor_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 floor_model = glm::mat4(1.0f);
    floor_model = glm::translate(floor_model, floor_pos);
    

    //      WALL
    glm::vec3 wall_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 wall_model = glm::mat4(1.0f);
    wall_model = glm::translate(wall_model, wall_pos);


    //      BOOKSHELF
    glm::vec3 bookshelf_pos = glm::vec3(-0.8f, 0.001f, 0.2f);
    glm::mat4 bookshelf_model = glm::mat4(1.0f);
    bookshelf_model = glm::translate(bookshelf_model, bookshelf_pos);
    // bookshelf_model = glm::rotate(bookshelf_model, 1.57f, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec3 table_pos = glm::vec3(3.0f, 0.001f, 0.2f);
    glm::mat4 table_model = glm::mat4(1.0f);
    table_model = glm::translate(table_model, table_pos);

    //      RUG
    glm::vec3 rug_pos = glm::vec3(-0.2f, 0.001f, 0.2f);
    glm::mat4 rug_model = glm::mat4(1.0f);
    rug_model = glm::translate(rug_model, rug_pos);



    
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


        pokeballShader.Activate();

        pokeballCenter.draw(pokeballShader,camera);
        pokeballLeft.draw(pokeballShader,camera);
        pokeballRight.draw(pokeballShader,camera);


        shader_program.Activate();
        bookshelf.Draw_mesh(bookshelf_model, shader_program, camera);
        bookshelf_top.Draw_mesh(bookshelf_model, shader_program, camera);
        bookshelf_side.Draw_mesh(bookshelf_model, shader_program, camera);
        
        floor.Draw_mesh(floor_model, shader_program, camera);
        wall.Draw_mesh(wall_model, shader_program, camera);

        rug.Draw_mesh(rug_model, shader_program, camera);
        rug_top.Draw_mesh(rug_model, shader_program, camera);

        
        table.draw(shader_program, camera);

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