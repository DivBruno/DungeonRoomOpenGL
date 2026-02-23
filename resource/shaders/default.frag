#version 330 core
out vec4 FragColor;

in vec3 crnt_pos;
in vec3 normal;
in vec3 color; 
in vec2 text_coord;

uniform vec4 baseColorFactor;
uniform bool hasTexture;

uniform sampler2D diffuse0;

uniform vec4 light_color;
uniform vec3 cam_pos;

vec4 direct_light()
{
   float ambient = 0.2;

   vec3 Normal = normalize(normal);
   vec3 light_direction = normalize(vec3(1.0, 1.0, 0.0));
   float diffuse = max(dot(Normal, light_direction), 0.0);

   float specular_light = 0.5;
   vec3 view_direction = normalize(cam_pos - crnt_pos);
   vec3 reflection_direction = reflect(-light_direction, Normal);
   float spec_amount = pow(max(dot(view_direction, reflection_direction), 0.0), 16);
   float specular = spec_amount * specular_light;

   vec4 baseColor;

   if (hasTexture)
       baseColor = texture(diffuse0, text_coord);
   else
       baseColor = baseColorFactor;

   return baseColor * (ambient + diffuse + specular) * light_color;
}

void main()
{
   FragColor = direct_light();
}