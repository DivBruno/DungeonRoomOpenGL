#include "Pokeball.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Pokeball::Pokeball(glm::vec3 pos, float s)
    : position(pos), scale(s), sphere(1.0f, 64, 64)
{
}

void Pokeball::draw(Shader& shader)
{
    shader.Activate();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(scale));

    glUniformMatrix4fv(
        glGetUniformLocation(shader.ID, "model"),
        1, GL_FALSE,
        glm::value_ptr(model)
    );

    sphere.Draw();
}
