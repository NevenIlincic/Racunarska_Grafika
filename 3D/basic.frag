#version 330 core

/*in vec4 channelCol;
in vec2 channelTex;

out vec4 outCol;

uniform sampler2D uTex;
uniform bool useTex;
uniform bool transparent;

void main()
{
	if (!useTex) {
		outCol = channelCol;
	}
	else {
		outCol = texture(uTex, channelTex);
		if (!transparent && outCol.a < 1) {
			outCol = vec4(1.0, 1.0, 1.0, 1.0);
		}
	}
}
*/

struct Light{ //Svjetlosni izvor
	vec3 pos; //Pozicija
	vec3 kA; //Ambijentalna komponenta (Indirektno svjetlo)
	vec3 kD; //Difuzna komponenta (Direktno svjetlo)
	vec3 kS; //Spekularna komponenta (Odsjaj)
};
struct Material{ //Materijal objekta
	vec3 kA;
	vec3 kD;
	vec3 kS;
	float shine; //Uglancanost
};

in vec3 chNormal;
in vec3 chFragPos;

out vec4 outCol;

uniform Light uLight;
uniform Light uNewLight;
uniform Material uMaterial;
uniform vec3 uViewPos;	//Pozicija kamere (za racun spekularne komponente)


void main()
{
	vec3 resA = uLight.kA * uMaterial.kA;

	vec3 normal = normalize(chNormal);
	vec3 lightDirection = normalize(uLight.pos - chFragPos);
	float nD = max(dot(normal, lightDirection), 0.0);
	vec3 resD = uLight.kD * ( nD * uMaterial.kD);

	vec3 viewDirection = normalize(uViewPos - chFragPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float s = pow(max(dot(viewDirection, reflectionDirection), 0.0), uMaterial.shine);
	vec3 resS = uLight.kS * (s * uMaterial.kS);

	/////
	/*vec3 resA2 = uNewLight.kA * uMaterial.kA;

	vec3 lightDir2 = normalize(uNewLight.pos - chFragPos);
	float diff2 = max(dot(normal, lightDir2), 0.0);
	vec3 resD2 = uNewLight.kD * (diff2 * uMaterial.kD);

	vec3 reflectDir2 = reflect(-lightDir2, normal);
	float spec2 = pow(max(dot(viewDirection, reflectDir2), 0.0), uMaterial.shine);
	vec3 resS2 = uNewLight.kS * (spec2 * uMaterial.kS);*/


	outCol = vec4(resA + resD + resS, 1.0); //Fongov model sjencenja
}

