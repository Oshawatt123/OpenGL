#version 450

struct GL_LightData
{
	vec3 position;
	float falloff;
	vec3 color;
};

const int numLights = 5;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;

uniform GL_LightData[numLights] lightData;
uniform vec3 FragCamPos;

in vec2 FragTextureCoord;
in vec3 FragNormal;
in vec3 FragPos;
in mat3 TBN;

out vec4 frag_colour;
void main() {


vec3 ambient = vec3(0);
vec3 diffuse = vec3(0);
vec3 specular = vec3(0);

vec3 normal = vec3(0);

for(int i = 0; i < numLights; i++)
{

	// ambient
	float ambientStrength = 0.1f;
	ambient += ambientStrength * lightData[i].color;

	// diffuse
	//vec3 normal = normalize(FragNormal);
	normal = (texture(texture_normal, FragTextureCoord).rbg);
	normal = normalize((normal * 2.0) - 1.0);
	normal = normalize(TBN*normal);

	float lightDistance = distance(lightData[i].position, FragPos);
	float falloffEffect = lightData[i].falloff / lightDistance;

	vec3 lightDir = normalize(lightData[i].position - FragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	diffuse += (diff * lightData[i].color) * falloffEffect;

	// specular
	float specularStrength = 80f;
	vec3 viewDir = normalize(FragCamPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	specular += vec3(specularStrength * spec) * falloffEffect;

}

	//vec4 result = vec4( vec3(FragTextureCoord.x, FragTextureCoord.y, 0), 1.0f);
	//vec4 result = vec4( vec3(FragTextureCoord.x, FragTextureCoord.y, 0) * (ambient + diff + spec), 1.0f);
	//vec4 result = vec4( vec3(FragTextureCoord.x, FragTextureCoord.y, 0) * (ambient + diff), 1.0);
	vec4 result = vec4(texture2D(texture_diffuse, FragTextureCoord).rgb * (ambient + diffuse + specular), 1);
	//vec4 result = vec4(normal, 1);
	//vec4 result = vec4((ambient + diffuse + spec), 1.0);
	//vec4 result = vec4(lightDir, 1);

	frag_colour = result;
}