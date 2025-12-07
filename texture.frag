#version 330 core

in vec2 chTex;
out vec4 outCol;

uniform sampler2D uTex;
uniform float uAlpha;

void main()
{
    vec4 texColor = texture(uTex, chTex);
    if (texColor.a < 0.1) 
        discard;
    texColor.a = uAlpha;
    outCol = texColor;
}