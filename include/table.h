#pragma once

#include "Mesh.h"
#include "shader_class.h"
#include "Camera.h"
#include <glm/glm.hpp>

class Table
{
private:
    glm::vec3 position;

    Mesh meshBranco; 
    Mesh meshVerde; 

public:
    Table(glm::vec3 pos,
          Mesh branco,
          Mesh verde);

    void draw(Shader& shader, Camera& camera);
};