#version 330 core

in vec4 channelCol;
in vec2 channelTex;

out vec4 outCol;

uniform sampler2D uTex;
uniform float uAlpha; // DODAJ OVO - šalješ ga iz Watermark.cpp

void main()
{
    vec4 texColor = texture(uTex, channelTex);
    outCol = vec4(texColor.rgb, texColor.a * uAlpha);
}