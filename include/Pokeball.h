#pragma once
#include <glm/glm.hpp>
#include "Mesh.h"
#include "shader_class.h"
#include "Camera.h"
class Pokeball {
public:
    glm::vec3 position;
    float scale;
    Mesh sphere;

    Pokeball(glm::vec3 pos, float s);

    // <-- adicione Camera como parâmetro
    void draw(Shader& shader, Camera& camera);
};