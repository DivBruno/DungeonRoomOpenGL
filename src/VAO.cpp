#include "VAO.H"

VAO::VAO(){
    glGenVertexArrays(1, &ID);

}

// changed to make it more flexible, right now is to make each vertices with different colors, 
// so now the VAO can be linked with more attributes
void VAO::LinkAttrib(VBO &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset){
    VBO.Bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    VBO.Unbind();
}

void VAO::Bind(){
    glBindVertexArray(ID);
}
void VAO::Unbind(){
    glBindVertexArray(0);
}
void VAO::Delete(){
    glDeleteVertexArrays(1, &ID);
}