#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) uniform sampler2D u_Screen;
layout (location = 1) uniform sampler2D u_ThresholdScreen;
layout (location = 2) uniform float u_Gamma;
layout (location = 3) uniform float u_Exposure;

const float weights[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

vec3 GaussianBlur(int lod)
{
    vec2 texOffset = 1.0 / textureSize(u_ThresholdScreen, 0) / (lod + 1);
    vec3 result = textureLod(u_ThresholdScreen, v_TexCoord, lod).rgb * weights[0];

    for (int i = 1; i < 5; i++)
    {
        result += textureLod(u_ThresholdScreen, v_TexCoord + vec2(texOffset.x * i, 0.0), lod).rgb * weights[i];
        result += textureLod(u_ThresholdScreen, v_TexCoord - vec2(texOffset.x * i, 0.0), lod).rgb * weights[i];
    }

    for (int i = 1; i < 5; i++)
    {
        result += textureLod(u_ThresholdScreen, v_TexCoord + vec2(texOffset.y * i, 0.0), lod).rgb * weights[i];
        result += textureLod(u_ThresholdScreen, v_TexCoord - vec2(texOffset.y * i, 0.0), lod).rgb * weights[i];
    }

    return result;
}

vec3 Bloom()
{
    vec3 color;
    color  = textureLod(u_ThresholdScreen, v_TexCoord, 0).rgb;

    for (int i = 1; i <= 6; i++)
    {
        color += textureLod(u_ThresholdScreen, v_TexCoord, i).rgb;
        //color += GaussianBlur(i);
    }

    return color;
}

void main()
{    
    vec3 col = textureLod(u_Screen, v_TexCoord, 0).rgb;

    col += Bloom();

    //vec3 mapped = col / (col + vec3(1.0));          //REINHARD TONE MAPPING
    
    vec3 mapped = vec3(1.0) - exp(-col * u_Exposure); //EXPOSURE TONE MAPPING
    mapped = pow(mapped, vec3(1.0 / u_Gamma));        //GAMMA CORRECTION

    f_Color = vec4(mapped, 1.0);
}