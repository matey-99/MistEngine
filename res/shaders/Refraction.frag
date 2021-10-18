#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;

layout (std140, binding = 1) uniform u_FragmentCamera
{
    vec3 u_CameraPosition;
};

layout (location = 1) uniform samplerCube u_Skybox;

// TO DO
//layout (location = 2) uniform float u_RefractiveIndex;

void main()
{
    //float ratio = 1.00 / u_RefractiveIndex;
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(v_Position - u_CameraPosition);
    vec3 R = refract(I, normalize(v_Normal), ratio);
    f_Color = vec4(texture(u_Skybox, R).rgb, 1.0);
}