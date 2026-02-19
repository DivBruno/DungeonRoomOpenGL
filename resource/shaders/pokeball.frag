#version 330 core

out vec4 FragColor;

in vec3 crnt_pos;
in vec3 normal;
in vec3 local_pos;

uniform vec4 light_color;
uniform vec3 light_pos;
uniform vec3 cam_pos;

void main()
{
    
    vec3 baseColor = vec3(1.0, 0.0, 0.0);

    // ================= PARÂMETROS PROPORCIONAIS =================
    float radius = length(local_pos);
    float bandThickness = radius * 0.03;
    float buttonRadius  = radius * 0.18;
    float buttonBorder  = radius * 0.25;
    float buttonDepth   = radius * 0.85;

    float dist = length(vec2(local_pos.x, local_pos.y));

    // ================= BOTÃO =================
    if (local_pos.z > buttonDepth)
    {
        if (dist < buttonRadius)
            baseColor = vec3(1.0, 1.0, 1.0);
        else if (dist < buttonBorder)
            baseColor = vec3(0.0, 0.0, 0.0);
        else
        {
            if (local_pos.y > bandThickness)
                baseColor = vec3(1.0, 0.0, 0.0);
            else if (local_pos.y < -bandThickness)
                baseColor = vec3(1.0, 1.0, 1.0);
            else
                baseColor = vec3(0.0, 0.0, 0.0);
        }
    }
    else
    {
        // ================= CORPO =================
        if (local_pos.y > bandThickness)
            baseColor = vec3(1.0, 0.0, 0.0);
        else if (local_pos.y < -bandThickness)
            baseColor = vec3(1.0, 1.0, 1.0);
        else
            baseColor = vec3(0.0, 0.0, 0.0);
    }
    // ================= ILUMINAÇÃO (IGUAL AO DEFAULT) =================

    float ambient = 0.2f;

    vec3 Normal = normalize(normal);
    vec3 light_direction = normalize(vec3(1.0f, 1.0f, 0.0f));

    float diffuse = max(dot(Normal, light_direction), 0.0f);

    float specular_light = 0.5f;
    vec3 view_direction = normalize(cam_pos - crnt_pos);
    vec3 reflection_direction = reflect(-light_direction, Normal);
    float spec_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 16);
    float specular = spec_amount * specular_light;

    vec3 finalColor = baseColor * (diffuse + ambient) + vec3(1.0) * specular;
    FragColor = vec4(finalColor, 1.0);
}
