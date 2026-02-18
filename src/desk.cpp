#include "desk.h"
#include <glm/gtc/matrix_transform.hpp>

Desk::Desk(Mesh* cubo, Mesh* troncoPiramide, glm::vec3 posicaoInicial)
    : cubo(cubo),
      troncoPiramide(troncoPiramide),
      posicao(posicaoInicial),
      rotacao(0.0f),
      escala(1.0f)
{
}

void Desk::DrawTampo(glm::mat4 base, Shader& shader, Camera& camera){
    glm::mat4 tampo = base;

    tampo = glm::translate(tampo,
        glm::vec3(0.0f, 0.35f, 0.01f)
    );
    tampo = glm::scale(tampo,
        glm::vec3(comprimento,
                  0.02f,
                 profundidade-0.01)
    );

    glUniformMatrix4fv(
        glGetUniformLocation(shader.ID, "model"),
        1,
        GL_FALSE,
        glm::value_ptr(tampo)
    );

    cubo->Draw(shader, camera);
}

void Desk::DrawPernaGavetas(glm::mat4 base, Shader& shader, Camera& camera){
    glm::mat4 pernaGaveta = base;
    pernaGaveta = glm::translate(pernaGaveta,
        glm::vec3(0.45f, -0.01f, 0.01f)
    );
    
    pernaGaveta = glm::scale(pernaGaveta,
        glm::vec3(0.5f,
            altura-0.02,
            profundidade-0.01)
    );

    glUniformMatrix4fv(
        glGetUniformLocation(shader.ID, "model"),
        1,
        GL_FALSE,
        glm::value_ptr(pernaGaveta)
    );
    cubo->Draw(shader, camera);

    glm::mat4 gaveta = base;
    gaveta = glm::translate(gaveta, glm::vec3(0.45f, 0.190f, 0.35));

    gaveta = glm::scale(gaveta,
        glm::vec3(9,
            6,
            3)
    );

    glUniformMatrix4fv(
        glGetUniformLocation(shader.ID, "model"),
        1,
        GL_FALSE,
        glm::value_ptr(gaveta)
    );
    troncoPiramide->Draw(shader, camera);

    glm::mat4 gaveta2 = base;
    gaveta2 = glm::translate(gaveta2, glm::vec3(0.45f, -0.18f, 0.35));

    gaveta2 = glm::scale(gaveta2,
        glm::vec3(9,
            6,
            3)
    );

    glUniformMatrix4fv(
        glGetUniformLocation(shader.ID, "model"),
        1,
        GL_FALSE,
        glm::value_ptr(gaveta2)
    );
    troncoPiramide->Draw(shader, camera);

    glm::mat4 basePuxador = base;
    basePuxador = glm::translate(basePuxador, glm::vec3(0.45f, 0.19f, 0.385));
    basePuxador = glm::scale(basePuxador,
        glm::vec3(0.01f,
            0.01f,
            0.01f)
    );

    glUniformMatrix4fv(
        glGetUniformLocation(shader.ID, "model"),
        1,
        GL_FALSE,
        glm::value_ptr(basePuxador)
    );
    cubo->Draw(shader, camera);

    glm::mat4 puxador = base;
    puxador = glm::translate(puxador, glm::vec3(0.45f, 0.19f, 0.39));

    glUniformMatrix4fv(
        glGetUniformLocation(shader.ID, "model"),
        1,
        GL_FALSE,
        glm::value_ptr(puxador)
    );
    troncoPiramide->Draw(shader, camera);

    glm::mat4 basePuxador2 = base;
    basePuxador2 = glm::translate(basePuxador2, glm::vec3(0.45f, -0.18f, 0.385));
    basePuxador2 = glm::scale(basePuxador2,
        glm::vec3(0.01f,
            0.01f,
            0.01f)
    );

    glUniformMatrix4fv(
        glGetUniformLocation(shader.ID, "model"),
        1,
        GL_FALSE,
        glm::value_ptr(basePuxador2)
    );
    cubo->Draw(shader, camera);

    glm::mat4 puxador2 = base;
    puxador2 = glm::translate(puxador2, glm::vec3(0.45f, -0.18f, 0.39));

    glUniformMatrix4fv(
        glGetUniformLocation(shader.ID, "model"),
        1,
        GL_FALSE,
        glm::value_ptr(puxador2)
    );
    troncoPiramide->Draw(shader, camera);
}

void Desk::DrawPerna(glm::mat4 base, Shader& shader, Camera& camera){
    glm::mat4 perna = base;
    perna = glm::translate(perna,
        glm::vec3(-0.675f, -0.01f, 0.01f)
    );
    
    perna = glm::scale(perna,
        glm::vec3(0.05f,
            altura-0.02,
            profundidade-0.01)
    );

    glUniformMatrix4fv(
        glGetUniformLocation(shader.ID, "model"),
        1,
        GL_FALSE,
        glm::value_ptr(perna)
    );
    cubo->Draw(shader, camera);
}

void Desk::DrawFundo(glm::mat4 base, Shader& shader, Camera& camera){
    glm::mat4 fundo = base;
    fundo = glm::translate(fundo,
        glm::vec3(0.0f, 0.0f, -0.34f)
    );
    
    fundo = glm::scale(fundo,
        glm::vec3(comprimento,
            altura,
            0.01)
    );

    glUniformMatrix4fv(
        glGetUniformLocation(shader.ID, "model"),
        1,
        GL_FALSE,
        glm::value_ptr(fundo)
    );
    cubo->Draw(shader, camera);
}

void Desk::Draw(Shader& shader, Camera& camera){
    shader.Activate();

    glm::mat4 base = glm::mat4(1.0f);
    base = glm::translate(base, posicao);
    base = glm::rotate(base, glm::radians(rotacao.y), glm::vec3(0,1,0));
    base = glm::scale(base, escala);

    DrawTampo(base, shader, camera);
    DrawPernaGavetas(base, shader, camera);
    DrawPerna(base, shader, camera);
    DrawFundo(base, shader, camera);
}