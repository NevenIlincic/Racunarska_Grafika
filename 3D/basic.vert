#version 330 core

/*layout(location = 0) in vec3 inPos;
layout(location = 1) in vec4 inCol;
layout(location = 2) in vec2 inTex;

uniform mat4 uM; //Matrica transformacije
uniform mat4 uV; //Matrica kamere
uniform mat4 uP; //Matrica projekcija

uniform vec3 uColor;    // boja kvadrata

out vec4 channelCol;
out vec2 channelTex;

void main()
{
	gl_Position = uP * uV * uM * vec4(inPos, 1.0); //Zbog nekomutativnosti mnozenja matrica, moramo mnoziti MVP matrice i tjemena "unazad"
	channelCol = vec4(uColor, 1.0);
	channelTex = inTex;
}
*/

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

out vec3 chFragPos;
out vec3 chNormal;
out vec2 chUV;

uniform mat4 uM;
uniform mat4 uV;
uniform mat4 uP;

void main()
{
    chUV = inUV;
    chFragPos = vec3(uM * vec4(inPos, 1.0));
    chNormal = mat3(transpose(inverse(uM))) * inNormal;  
    
    gl_Position = uP * uV * vec4(chFragPos, 1.0);
}

