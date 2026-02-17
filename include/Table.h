#pragma once
#include "shader_class.h"
#include "VAO.h"
#include <glm/glm.hpp>
#include "Texture.h"
class Table {
public:
    glm::vec3 position;

    Table(glm::vec3 pos);
    void draw(Shader& shader, VAO& vao, int indexCount,
        Texture& mesa, Texture& mesaInterna);
};
