#version 450 core

#define MAX_POINT_LIGHTS 64
#define MAX_SPOT_LIGHTS 64

layout (location = 0) out vec4 f_Color;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
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
layout (location = 2) in vec2 v_TexCoord;
layout (location = 3) in vec3 v_TangentViewPosition;
layout (location = 4) in vec3 v_TangentPosition;
layout (location = 5) in mat3 v_TBN;

layout (location = 3) uniform Material u_Material;

layout (location = 7) uniform int u_PointLightsCount;
layout (location = 8) uniform int u_SpotLightsCount;

layout (location = 9) uniform DirectionalLight u_DirectionalLight;
layout (location = 13) uniform PointLight[MAX_POINT_LIGHTS] u_PointLights;
layout (location = 13 + MAX_POINT_LIGHTS * 6) uniform SpotLight[MAX_SPOT_LIGHTS] u_SpotLights;

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDirection = normalize(-light.direction);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), u_Material.shininess);

    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, v_TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_Material.diffuse, v_TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, v_TexCoord));

    return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDirection)
{
    vec3 tangentLightPosition = v_TBN * light.position;
    vec3 lightDirection = normalize(tangentLightPosition - v_TangentPosition);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float spec = pow(max(dot(viewDirection, halfwayDirection), 0.0), u_Material.shininess);

    float dist = length(tangentLightPosition - v_TangentPosition);
    float attenuation = 1.0 / (1.0 + light.linear * dist + light.quadratic * (dist * dist));

    vec3 ambient = light.ambient * attenuation * vec3(texture(u_Material.diffuse, v_TexCoord));
    vec3 diffuse = light.diffuse * diff * attenuation * vec3(texture(u_Material.diffuse, v_TexCoord));
    vec3 specular = light.specular * spec * attenuation * vec3(texture(u_Material.specular, v_TexCoord));

    return (ambient + diffuse + specular);
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 viewDirection)
{
    vec3 tangentLightPosition = v_TBN * light.position;
    vec3 lightDirection = normalize(tangentLightPosition - v_TangentPosition);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float spec = pow(max(dot(viewDirection, halfwayDirection), 0.0), u_Material.shininess);

    float dist = length(tangentLightPosition - v_TangentPosition);
    float attenuation = 1.0 / (1.0 + light.linear * dist + light.quadratic * (dist * dist));

    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * intensity * attenuation * vec3(texture(u_Material.diffuse, v_TexCoord));
    vec3 diffuse = light.diffuse * diff * intensity * attenuation * vec3(texture(u_Material.diffuse, v_TexCoord));
    vec3 specular = light.specular * spec * intensity * attenuation * vec3(texture(u_Material.specular, v_TexCoord));

    return (ambient + diffuse + specular);
}

void main()
{    
    vec3 normal = texture(u_Material.normal, v_TexCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    vec3 viewDirection = normalize(v_TangentViewPosition - v_TangentPosition);

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