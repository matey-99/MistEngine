#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec3 v_TexCoord;

layout (location = 1) uniform samplerCube u_Skybox;

void main()
{    
    f_Color = texture(u_Skybox, v_TexCoord);
}