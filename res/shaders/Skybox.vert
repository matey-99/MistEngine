#version 450 core

layout (location = 0) in vec3 a_Position;

layout (location = 0) out vec3 v_TexCoord;

layout (location = 0) uniform mat4 u_ViewProjection;

void main()
{
    v_TexCoord = a_Position;
    vec4 pos = u_ViewProjection * vec4(a_Position, 1.0);
    gl_Position = pos.xyww;
}