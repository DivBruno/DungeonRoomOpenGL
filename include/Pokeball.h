#pragma once
#include "shader_class.h"
#include "Sphere.h"
#include <glm/glm.hpp>

class Pokeball
{
public:
    glm::vec3 position;
    float scale;
    Sphere sphere;

    Pokeball(glm::vec3 pos, float s);

    void draw(Shader& shader);
};
