#version 450 core

layout (location = 0) out vec4 f_Color;

struct Material
{
    sampler2D diffuse1;
    sampler2D specular1;
    float shininess;
};

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec2 v_TexCoord;

layout (location = 3) uniform vec3 u_ViewPosition;
layout (location = 4) uniform Material u_Material;
layout (location = 7) uniform Light u_Light;

void main()
{    
    vec3 ambient = u_Light.ambient * vec3(texture(u_Material.diffuse1, v_TexCoord));

    vec3 normal = normalize(v_Normal);
    vec3 lightDirection = normalize(u_Light.position - v_Position);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = u_Light.diffuse * diff * vec3(texture(u_Material.diffuse1, v_TexCoord));

    vec3 viewDirection = normalize(u_ViewPosition - v_Position);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), u_Material.shininess);
    vec3 specular = u_Light.specular * spec * vec3(texture(u_Material.specular1, v_TexCoord));

    f_Color = vec4(ambient + diffuse + specular, 1.0);
}