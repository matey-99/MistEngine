#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

out vec3 v_Position;
out vec3 v_Normal;
out vec2 v_TexCoord;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
    v_Position = vec3(u_Model * vec4(a_Position, 1.0));
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
    v_TexCoord = a_TexCoord;

    gl_Position = u_Projection * u_View * vec4(v_Position, 1.0);
}