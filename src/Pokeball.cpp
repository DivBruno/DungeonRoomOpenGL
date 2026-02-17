#include "Pokeball.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"

Pokeball::Pokeball(glm::vec3 pos, float s)
    : position(pos), scale(s),sphere(Mesh::GenerateSphere(1.0f, 64, 64))
{
    
}

void Pokeball::draw(Shader& shader, Camera& camera)
{
    shader.Activate();

    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model = glm::scale(model, glm::vec3(scale));

    glUniformMatrix4fv(
        glGetUniformLocation(shader.ID, "model"),
        1, GL_FALSE,
        glm::value_ptr(model)
    );
    camera.Matrix(shader, "camMatrix");

    sphere.Draw(shader, camera);
}
