#version 330 core

layout(location = 0) in vec2 inPos;   // verteks pozicija

out vec3 fragColor; // boja koja ide u fragment shader

// Uniforms za poziciju, skalu i boju kvadrata
uniform float uX;       // X pozicija centra kvadrata
uniform float uY;       // Y pozicija centra kvadrata
uniform float uSx;      // skala po X
uniform float uSy;      // skala po Y
uniform vec3 uColor;    // boja kvadrata

void main()
{
    // Pomeri i skaliraj kvadrat
    gl_Position = vec4(inPos.x * uSx + uX, inPos.y * uSy + uY, 0.0, 1.0);
    
    // Prosledi boju fragment shaderu
    fragColor = uColor;
}
