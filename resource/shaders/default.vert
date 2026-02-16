#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; 
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aText;

out vec3 crnt_pos;
out vec3 normal;
out vec3 color;
out vec2 text_coord;


uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
   crnt_pos = vec3(model * vec4(aPos, 1.0f));

   normal = aNormal;
   color = aColor;
   text_coord = aText;

   gl_Position = camMatrix * vec4(crnt_pos, 1.0);
}