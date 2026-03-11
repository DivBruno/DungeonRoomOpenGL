#include "Texture.h"
#include "table.h"
#include "pokeball.h"

Texture::Texture(const char* image, const char* texType, GLuint slot, GLenum format, GLenum pixelType){
    type = texType;
    int img_width, img_height, img_num_col_ch;
    unsigned char* bytes = stbi_load(image, &img_width, &img_height, &img_num_col_ch, STBI_rgb_alpha);

    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + slot);
    unit = slot;
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float back_color[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, back_color);
 

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, format, pixelType, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::texUnit(Shader &shader, const char *uniform, GLuint unit){
    GLuint tex0_uni = glGetUniformLocation(shader.ID, uniform);
    shader.Activate();
    glUniform1i(tex0_uni, unit);
}

void Texture::Bind(){
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind(){
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete(){
    glDeleteTextures(1, &ID);
}