#pragma once
#include <vector>
#include <glad/glad.h>

class Sphere
{
public:
    unsigned int VAO, VBO, EBO;
    unsigned int indexCount;

    Sphere(float radius, unsigned int sectors, unsigned int stacks);

    void Draw();
};