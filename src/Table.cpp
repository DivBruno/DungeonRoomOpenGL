#include "Table.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Table::Table(glm::vec3 pos,
             Mesh branco,
             Mesh verde)
    : position(pos),
      meshBranco(branco),
      meshVerde(verde)
{
}

void Table::draw(Shader& shader, Camera& camera)
{
    shader.Activate();
    GLuint modelLoc = glGetUniformLocation(shader.ID, "model");

    // =========================
    // ESCALAS
    // =========================
    glm::vec3 topScale = glm::vec3(1.5f, 0.1f, 1.0f);
    glm::vec3 legScale = glm::vec3(0.15f, 0.2f, 0.15f);

    float margin = 0.06f;
    float topCenterY = -0.5f;

    // =========================
    // TAMPO EXTERNO (BRANCO)
    // =========================
    glm::mat4 top = glm::mat4(1.0f);
    top = glm::translate(top, position + glm::vec3(0.0f, topCenterY, 0.0f));
    top = glm::scale(top, topScale);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(top));
    meshBranco.Draw(shader, camera);

    // =========================
    // TAMPO INTERNO (VERDE)
    // =========================
    glm::vec3 innerScale = glm::vec3(1.42f, 0.039f, 0.94f);

    glm::mat4 inside = glm::mat4(1.0f);
    inside = glm::translate(inside, position + glm::vec3(0.0f, topCenterY + 0.05f, 0.0f));
    inside = glm::scale(inside, innerScale);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(inside));
    meshVerde.Draw(shader, camera);

    // =========================
    // PERNAS (BRANCO)
    // =========================
    float topHalfHeight = topScale.y * 0.5f;
float legHalfHeight = legScale.y * 0.5f;

float topBottom = topCenterY + (-0.4f * topScale.y);
float legCenterY = topBottom - (0.8f * legScale.y);

float halfX = (topScale.x * 0.4f) - margin;
float halfZ = (topScale.z * 0.4f)  - margin;

glm::vec3 legPositions[] = {
    {-halfX, legCenterY,  halfZ},
    { halfX, legCenterY,  halfZ},
    {-halfX, legCenterY, -halfZ},
    { halfX, legCenterY, -halfZ}
};

    for (int i = 0; i < 4; i++)
    {
        glm::mat4 leg = glm::mat4(1.0f);
        leg = glm::translate(leg, position + legPositions[i]);
        leg = glm::scale(leg, legScale);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(leg));
        meshBranco.Draw(shader, camera); // mesma textura do tampo externo
    }
}
