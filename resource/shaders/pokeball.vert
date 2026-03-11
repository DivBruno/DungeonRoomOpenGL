#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 crnt_pos;
out vec3 normal;
out vec3 local_pos;   

uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
    local_pos = aPos;  

    crnt_pos = vec3(model * vec4(aPos, 1.0));
    normal = mat3(transpose(inverse(model))) * aNormal;

    gl_Position = camMatrix * model * vec4(aPos, 1.0);
}
