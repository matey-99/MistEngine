#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) uniform sampler2D u_Screen;
layout (location = 1) uniform float u_Gamma;
layout (location = 2) uniform float u_Exposure;

void main()
{    
    vec3 col = texture(u_Screen, v_TexCoord).rgb;

    //vec3 mapped = col / (col + vec3(1.0));          //REINHARD TONE MAPPING
    vec3 mapped = vec3(1.0) - exp(-col * u_Exposure); //EXPOSURE TONE MAPPING
    mapped = pow(mapped, vec3(1.0 / u_Gamma));        //GAMMA CORRECTION

    f_Color = vec4(mapped, 1.0);
}