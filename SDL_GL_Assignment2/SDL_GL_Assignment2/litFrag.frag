#version 450

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;

uniform vec3 FragLightColor;
uniform vec3 FragLightPos;
uniform vec3 FragCamPos;

in vec2 FragTextureCoord;
in vec3 FragNormal;
in vec3 FragPos;
in mat3 TBN;

out vec4 frag_colour;

void main() {

	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * FragLightColor;

	vec3 normal = (texture(texture_normal, FragTextureCoord).rbg);
	normal = normalize(normal * 2 - 1);
	normal = normalize(TBN * normal);

	vec3 lightDir = normalize(FragLightPos - FragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * FragLightColor;

	vec3 result = texture2D(texture_diffuse, FragTextureCoord).rgb * (ambient + diffuse);
	frag_colour = vec4(result, 1.0);	
}