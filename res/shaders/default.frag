#version 330 core

layout(location = 0) out vec4 f_Color;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 v_Position;
in vec3 v_Normal;

uniform vec3 u_ViewPosition;
uniform Material u_Material;
uniform Light u_Light;

void main()
{
    vec3 ambient = u_Light.ambient * u_Material.ambient;

    vec3 normal = normalize(v_Normal);
    vec3 lightDirection = normalize(u_Light.position - v_Position);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = u_Light.diffuse * (diff * u_Material.diffuse);

    vec3 viewDirection = normalize(u_ViewPosition - v_Position);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), u_Material.shininess);
    vec3 specular = u_Light.specular * (spec * u_Material.specular);

    f_Color = vec4(ambient + diffuse + specular, 1.0);
}