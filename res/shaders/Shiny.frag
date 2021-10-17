#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;

layout (location = 3) uniform vec3 u_CameraPosition;
layout (location = 4) uniform samplerCube u_Skybox;

void main()
{
    vec3 I = normalize(v_Position - u_CameraPosition);
    vec3 R = reflect(I, normalize(v_Normal));
    f_Color = vec4(texture(u_Skybox, R).rgb, 1.0);
}