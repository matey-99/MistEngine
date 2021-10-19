#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) uniform sampler2D u_Screen;

void main()
{    
    float gamma = 2.2;
    vec3 col = texture(u_Screen, v_TexCoord).rgb;
    col.rgb = pow(col.rgb, vec3(1.0 / gamma));
    f_Color = vec4(col, 1.0);
}