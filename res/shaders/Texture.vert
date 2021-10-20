#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in vec3 a_Tangent;

layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec3 v_Normal;
layout (location = 2) out vec2 v_TexCoord;
layout (location = 3) out vec3 v_TangentViewPosition;
layout (location = 4) out vec3 v_TangentPosition;
layout (location = 5) out mat3 v_TBN;

layout (std140, binding = 0) uniform u_Camera
{
    mat4 u_ViewProjection;
};

layout (location = 0) uniform mat4 u_Model;
layout (location = 1) uniform vec3 u_LightPosition;
layout (location = 2) uniform vec3 u_ViewPosition;

void main()
{
    v_Position = vec3(u_Model * vec4(a_Position, 1.0));
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
    v_TexCoord = a_TexCoord;

    mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
    vec3 T = normalize(normalMatrix * a_Tangent);
    vec3 N = normalize(normalMatrix * a_Normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    v_TBN = transpose(mat3(T, B, N));
    v_TangentViewPosition = v_TBN * u_ViewPosition;
    v_TangentPosition = v_TBN * v_Position;

    gl_Position = u_ViewProjection * vec4(v_Position, 1.0);
}