#version 330 core
out vec4 FragColor;

#define MAX_LIGHTS 5

in vec3 crnt_pos;
in vec3 normal;
in vec3 color; 
in vec2 text_coord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec4 light_color[MAX_LIGHTS];
uniform vec3 light_pos[MAX_LIGHTS];
uniform vec3 cam_pos;


vec4 point_light(){
   vec3 Normal = normalize(normal);
   vec3 view_direction = normalize(cam_pos - crnt_pos);

   vec4 final_color = vec4(0.0);

   for (int i = 0; i < MAX_LIGHTS; i++){
      vec3 light_vector = light_pos[i] - crnt_pos;

      float dist = length(light_vector);
      float a = 0.6; // quadratic term, the bigger it is, the faster the light weakens in big distances
      float b = 0.6; // linear tearm, the bigger it is, the faster the light weakens in medium distances
      float intensity = 1.0f / (a * dist * dist  + b * dist + 1.0f);

      vec3 light_direction = normalize(light_vector);

      float ambient = 0.1f;

      float diffuse = max(dot(Normal, light_direction), 0.0f);

      float specular_light = 0.5f;
      vec3 reflection_direction = reflect(-light_direction, Normal);
      float spec_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 16);
      float specular = spec_amount * specular_light;

      vec4 tex_diffuse = texture(diffuse0, text_coord);
      float tex_spec = 0.5;
      // Antigo, não deu muito certo então parei de usar float tex_spec = texture(specular0, text_coord).r;

      vec4 light_final = (tex_diffuse * (diffuse * intensity + ambient)
                        + tex_spec * specular * intensity)
                        * light_color[i];

      final_color += light_final;
   }

   final_color = clamp(final_color, 0.0f, 1.0f);

   return final_color;
}


/*
vec4 direct_light(){

   vec4 final_color = vec4(0.0);

   for (int i = 0; i < MAX_LIGHTS; i++){

      float ambient = 0.2f;

      vec3 Normal = normalize(normal);
      vec3 light_direction = normalize(vec3(1.0f, 1.0f, 0.0f));
      float diffuse = max(dot(Normal, light_direction), 0.0f);

      float specular_light = 0.5f;

      vec3 view_direction = normalize(cam_pos - crnt_pos);
      vec3 reflection_direction = reflect(-light_direction, Normal);
      float spec_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 16);
      float specular = spec_amount * specular_light;

      final_color +=
         (texture(diffuse0, text_coord) * (diffuse + ambient)
         + texture(specular0, text_coord).r * specular)
         * light_color[i];
   }

   return final_color;
}
*/


vec4 spot_light()
{
    vec3 Normal = normalize(normal);
    vec3 view_direction = normalize(cam_pos - crnt_pos);

    vec4 final_color = vec4(0.0);

    float outer_cone = 0.9;
    float inner_cone = 0.95;
    float ambient = 0.1;
    float specular_light = 0.5;

    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        vec3 light_vector = light_pos[i] - crnt_pos;
        vec3 light_direction = normalize(light_vector);

        float diffuse = max(dot(Normal, light_direction), 0.0);

        vec3 reflection_direction = reflect(-light_direction, Normal);
        float spec_amount = pow(max(dot(view_direction, reflection_direction), 0.0), 16);
        float specular = spec_amount * specular_light;

        float angle = dot(vec3(0.0, -1.0, 0.0), -light_direction);
        float intensity = clamp((angle - outer_cone) / (inner_cone - outer_cone), 0.0, 1.0);

        vec4 tex_diffuse = texture(diffuse0, text_coord);
        float tex_spec = 0.5; // mantendo seu padrão atual

        vec4 result =
            (tex_diffuse * (diffuse * intensity + ambient)
            + tex_spec * specular * intensity)
            * light_color[i];

        final_color += result;
    }

    return clamp(final_color, 0.0, 1.0);
}



void main()
{
   FragColor = point_light();
}