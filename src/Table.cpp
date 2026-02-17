#include "Table.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Texture.h"

Table::Table(glm::vec3 pos)
{
    position = pos;
}

void Table::draw(Shader& shader, VAO& vao, int indexCount, Texture& mesa, Texture& mesaInterna)
{
    shader.Activate();
    vao.Bind();

    
    GLuint modelLoc = glGetUniformLocation(shader.ID, "model");

    // =========================
    // ESCALAS
    // =========================
    glm::vec3 topScale = glm::vec3(1.5f, 0.1f, 1.0f);
    glm::vec3 legScale = glm::vec3(0.15f, 0.2f, 0.15f);

    float margin = 0.06f;

    // =========================
    // TAMPO PRINCIPAL
    // =========================
    float topCenterY = -0.5f;
    mesa.Bind();
    glm::mat4 top = glm::mat4(1.0f);
    top = glm::translate(top, position + glm::vec3(0.0f, topCenterY, 0.0f));
    top = glm::scale(top, topScale);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(top));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    // =========================
    // TAMPO INTERNO 
    // =========================
    mesaInterna.Bind();
    glm::vec3 innerScale = glm::vec3(1.42f, 0.039f, 0.94f);

    glm::mat4 inside = glm::mat4(1.0f);
    inside = glm::translate(inside, position + glm::vec3(0.0f, topCenterY + 0.05f, 0.0f));
    inside = glm::scale(inside, innerScale);

    

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(inside));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    // =========================
    // POSIÇÃO DAS PERNAS (igual você já tinha)
    // =========================

    float topBottom = topCenterY + (-0.4f * topScale.y);
    float legCenterY = topBottom - (0.8f * legScale.y);

    float halfX = 0.4f * topScale.x - margin;
    float halfZ = 0.4f * topScale.z - margin;

    glm::vec3 legPositions[] = {
        {-halfX, legCenterY,  halfZ},
        { halfX, legCenterY,  halfZ},
        {-halfX, legCenterY, -halfZ},
        { halfX, legCenterY, -halfZ}
    };
    mesa.Bind();
    for (int i = 0; i < 4; i++)
    {
        glm::mat4 leg = glm::mat4(1.0f);
        leg = glm::translate(leg, position + legPositions[i]);
        leg = glm::scale(leg, legScale);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(leg));
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    }
}
