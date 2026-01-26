#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTex;
out vec2 chTex;

uniform float uX;
uniform float uY;
uniform float uZ;
uniform float uSx;
uniform float uSy;
uniform float uSz;

void main()
{
    gl_Position = vec4(inPos.x * uSx + uX, inPos.y * uSy + uY, inPos.z * uSz + uZ, 1.0);
    chTex = inTex;
}