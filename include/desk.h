#pragma once

#include "Mesh.h"
#include "shader_class.h"
#include "Camera.h"
#include <glm/glm.hpp>

class Desk
{
private:
    Mesh* cubo;
    Mesh* troncoPiramide;
    glm::vec3 posicao;
    glm::vec3 rotacao;
    glm::vec3 escala;

    void DrawTampo(glm::mat4 base, Shader& shader, Camera& camera);
    void DrawPernaGavetas(glm::mat4 base, Shader& shader, Camera& camera);
    void DrawPerna(glm::mat4 base, Shader& shader, Camera& camera);
    void DrawFundo(glm::mat4 base, Shader& shader, Camera& camera);
    const float altura = 0.72f, profundidade = 0.7f, comprimento = 1.4f;

public:
    Desk(Mesh* cubo, Mesh* troncoPiramide, glm::vec3 posicaoInicial);
    void Draw(Shader& shader, Camera& camera);
};