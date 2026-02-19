#include "Mesh.h"
#define PI 3.14159265359f

Mesh::Mesh(std::vector <Vertex> &vertices, std::vector <GLuint> &indices, std::vector <Texture> &textures){
    Mesh::vertices = vertices;
    Mesh::indices = indices;
    Mesh::textures = textures;

    VAO.Bind();

    VBO VBO(vertices);
    EBO EBO(indices);

    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*) 0);
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
    VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();
}
Mesh::Mesh(std::vector <Vertex> &vertices, std::vector <GLuint> &indices){
    Mesh::vertices = vertices;
    Mesh::indices = indices;
    Mesh::textures.clear();

    VAO.Bind();

    VBO VBO(vertices);
    EBO EBO(indices);

    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*) 0);
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
    VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();
}

Mesh Mesh::GenerateSphere(float radius, unsigned int sectors, unsigned int stacks) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    float x, y, z, xy;
    float sectorStep = 2 * PI / sectors;
    float stackStep  = PI / stacks;
    float sectorAngle, stackAngle;

    for (unsigned int i = 0; i <= stacks; ++i) {
        stackAngle = PI / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        y  = radius * sinf(stackAngle);

        for (unsigned int j = 0; j <= sectors; ++j) {
            sectorAngle = j * sectorStep;

            x = xy * cosf(sectorAngle);
            z = xy * sinf(sectorAngle);

            Vertex vertex;
            vertex.position = glm::vec3(x, y, z);
            vertex.normal   = glm::vec3(x / radius, y / radius, z / radius);
            vertex.color    = glm::vec3(1.0f);
            vertex.texUV    = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
    }

    for (unsigned int i = 0; i < stacks; ++i) {
        unsigned int k1 = i * (sectors + 1);
        unsigned int k2 = k1 + sectors + 1;

        for (unsigned int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    std::vector<Texture> empty_textures;
    return Mesh(vertices, indices, empty_textures);
}

void  Mesh::Draw(Shader &shader, Camera &camera){
    shader.Activate();
    VAO.Bind();

    unsigned int numDiffuse  = 0;
    unsigned int numSpecular = 0;

    for (unsigned   int i = 0; i < textures.size(); i++){
        std::string num;
        std::string type = textures[i].type;
        if (type == "diffuse"){
            num = std::to_string(numDiffuse++);
        }
        else if (type == "specular"){
            num = std::to_string(numSpecular++);
        }
        glActiveTexture(GL_TEXTURE0 + i);
        textures[i].texUnit(shader, (type + num).c_str(), i);
        textures[i].Bind();
    }

    glUniform3f(glGetUniformLocation(shader.ID, "cam_pos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");

    glDrawElements(GL_TRIANGLES,  indices.size(), GL_UNSIGNED_INT, 0);

}

void Mesh::Draw_mesh(const glm::mat4 &model, Shader &shader, Camera &camera) {
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    Draw(shader, camera);
}
    