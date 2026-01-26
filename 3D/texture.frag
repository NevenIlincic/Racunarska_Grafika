#version 330 core
in vec4 channelCol;
in vec2 channelTex;

out vec4 outCol;

uniform sampler2D uTex;
uniform float uAlpha; // DODAJ OVO - šalješ ga iz Watermark.cpp
in vec2 chUV; // Ovo mora biti isto ime kao gore!

void main() {
    vec4 texColor = texture(uTex, chUV);
    outCol = vec4(texColor.rgb, texColor.a * uAlpha);
}