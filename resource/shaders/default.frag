#version 330 core
out vec4 FragColor;


in vec3 crnt_pos;
in vec3 normal;
in vec3 color; 
in vec2 text_coord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec4 light_color;
uniform vec3 light_pos;
uniform vec3 cam_pos;


vec4 point_light(){
   vec3 light_vector = light_pos - crnt_pos;

   float dist = length(light_vector);
   float a = 1.0; // quadratic term, the bigger it is, the faster the light weakens in big distances
   float b = 0.1; // linear tearm, the bigger it is, the faster the light weakens in medium distances
   float intensity = 1.0f / (a * dist * dist  + b * dist + 1.0f);


   float ambient = 0.2f;

   vec3 Normal = normalize(normal);
   vec3 light_direction = normalize(light_vector);
   float diffuse = max(dot(Normal, light_direction), 0.0f);

   float specular_light = 0.5f;
   vec3 view_direction = normalize(cam_pos - crnt_pos);
   vec3 reflection_direction = reflect(-light_direction, Normal);
   float spec_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 16);
   float specular = spec_amount * specular_light;

   return (texture(diffuse0, text_coord) * (diffuse * intensity + ambient) + texture(specular0, text_coord).r * specular * intensity) * light_color;
}

vec4 direct_light(){
   float ambient = 0.2f;

   vec3 Normal = normalize(normal);
   vec3 light_direction = normalize(vec3(1.0f, 1.0f, 0.0f));
   float diffuse = max(dot(Normal, light_direction), 0.0f);

   float specular_light = 0.5f;
   vec3 view_direction = normalize(cam_pos - crnt_pos);
   vec3 reflection_direction = reflect(-light_direction, Normal);
   float spec_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 16);
   float specular = spec_amount * specular_light;

   return (texture(diffuse0, text_coord) * (diffuse + ambient) + texture(specular0, text_coord).r * specular) * light_color;
}

vec4 spot_light(){
   float outer_cone = 0.9f;
   float inner_cone = 0.95f;

   float ambient = 0.1f;

   vec3 Normal = normalize(normal);
   vec3 light_direction = normalize(light_pos - crnt_pos);
   float diffuse = max(dot(Normal, light_direction), 0.0f);

   float specular_light = 0.5f;
   vec3 view_direction = normalize(cam_pos - crnt_pos);
   vec3 reflection_direction = reflect(-light_direction, Normal);
   float spec_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 16);
   float specular = spec_amount * specular_light;

   float angle = dot(vec3(0.0f, -1.0f, 0.0f), -light_direction);
   float intensity = clamp((angle - outer_cone) / (inner_cone - outer_cone), 0.0f, 1.0f);

   return (texture(diffuse0, text_coord) * (diffuse * intensity + ambient) + texture(specular0, text_coord).r * specular * intensity) * light_color;
}

void main()
{
   FragColor = point_light();
}