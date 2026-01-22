#version 330 core

in vec3 fragColor;     // boja iz vertex shader-a
out vec4 outCol;       // izlaz fragmenta (boja piksela)

uniform float uAlpha;


void main()
{
    outCol = vec4(fragColor, uAlpha); // postavi alfa na 1 (neprozirno)
}
