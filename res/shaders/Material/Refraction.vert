#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;

layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec3 v_Normal;

layout (std140, binding = 0) uniform u_VertexCamera
{
    mat4 u_ViewProjection;
};

layout (location = 0) uniform mat4 u_Model;

void main()
{
    v_Position = vec3(u_Model * vec4(a_Position, 1.0));
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;

    gl_Position = u_ViewProjection * vec4(v_Position, 1.0);
}