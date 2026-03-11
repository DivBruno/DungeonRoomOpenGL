#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <string>

#include "VAO.H"
#include "EBO.H"
#include "Camera.H"
#include "Texture.H"

class Mesh{
    public:
    std::vector <Vertex> vertices;
    std::vector <GLuint> indices;
    std::vector <Texture> textures;

    glm::vec4 baseColorFactor = glm::vec4(1.0f);

    VAO VAO;

    Mesh(std::vector <Vertex> &vertices, std::vector <GLuint> &indices, std::vector <Texture> &textures);
    Mesh(std::vector <Vertex> &vertices, std::vector <GLuint> &indices);

    static Mesh GenerateSphere(float radius, unsigned int sectors, unsigned int stacks);
    static Mesh GenerateCube(float size);
    void Draw_mesh(const glm::mat4 &model, Shader &shader, Camera &camera);
    void Draw(Shader &shader, Camera &camera);
};



#endif