#version 450 core

#define MAX_POINT_LIGHTS 32
#define MAX_SPOT_LIGHTS 32

layout (location = 0) out vec4 f_Color;

struct Material
{
    vec3 color;
    float shininess;
};

struct DirectionalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float linear;
    float quadratic;

    float innerCutOff;
    float outerCutOff;
};

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;

layout (std140, binding = 1) uniform u_FragmentCamera
{
    vec3 u_ViewPosition;
};

layout (std140, binding = 2) uniform u_Lights
{
    int u_PointLightsCount;
    int u_SpotLightsCount;

    DirectionalLight u_DirectionalLight;
    PointLight[MAX_POINT_LIGHTS] u_PointLights;
    SpotLight[MAX_SPOT_LIGHTS] u_SpotLights;
};

layout (location = 2) uniform Material u_Material;

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDirection = normalize(-light.direction);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), u_Material.shininess);

    vec3 ambient = light.ambient * u_Material.color;
    vec3 diffuse = light.diffuse * diff * u_Material.color;
    vec3 specular = light.specular * spec * u_Material.color;

    return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDirection = normalize(light.position - v_Position);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float spec = pow(max(dot(viewDirection, halfwayDirection), 0.0), u_Material.shininess);

    float dist = length(light.position - v_Position);
    float attenuation = 1.0 / (1.0 + light.linear * dist + light.quadratic * (dist * dist));

    vec3 ambient = light.ambient * attenuation * u_Material.color;
    vec3 diffuse = light.diffuse * diff * attenuation * u_Material.color;
    vec3 specular = light.specular * spec * attenuation * u_Material.color;

    return (ambient + diffuse + specular);
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDirection = normalize(light.position - v_Position);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float spec = pow(max(dot(viewDirection, halfwayDirection), 0.0), u_Material.shininess);

    float dist = length(light.position - v_Position);
    float attenuation = 1.0 / (1.0 + light.linear * dist + light.quadratic * (dist * dist));

    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * intensity * attenuation * u_Material.color;
    vec3 diffuse = light.diffuse * diff * intensity * attenuation * u_Material.color;
    vec3 specular = light.specular * spec * intensity * attenuation * u_Material.color;

    return (ambient + diffuse + specular);
}

void main()
{
    vec3 normal = normalize(v_Normal);
    vec3 viewDirection = normalize(u_ViewPosition - v_Position);

    vec3 result = CalculateDirectionalLight(u_DirectionalLight, normal, viewDirection);

    for (int i = 0; i < u_PointLightsCount; i++)
    {
        result += CalculatePointLight(u_PointLights[i], normal, viewDirection);
    }
    for (int i = 0; i < u_SpotLightsCount; i++)
    {
        result += CalculateSpotLight(u_SpotLights[i], normal, viewDirection);
    }

    f_Color = vec4(result, 1.0);
}