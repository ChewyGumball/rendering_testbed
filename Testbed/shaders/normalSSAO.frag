#version 330 core

const int MAX_LIGHTS = 2;
struct PointLight {
	vec3 position;
	vec3 intensity;
};
uniform PointLight pointLights[MAX_LIGHTS];

uniform vec3 cameraPosition;
uniform int lightCount;

in vec3 FragPos;
in vec3 Normal;

layout(location = 0) out vec4 colour;
layout(location = 1) out vec4 ssao;

vec3 pointLightContribution(PointLight light, vec3 normal, vec3 position, vec3 viewDirection)
{
	vec3 toLight = light.position - position;
	vec3 lightDirection = normalize(toLight);
	float diffusePower = max(dot(normal, lightDirection), 0.0);

	vec3 reflectedDirection = reflect(-lightDirection, normal);
	vec3 h = normalize(lightDirection + viewDirection);
	float ndoth = dot(normal, h);
	float specularPower = pow(clamp(ndoth,0.0,1.0), 512);
	//float specularPower = pow(max(dot(viewDirection, reflectedDirection), 0.0), 256);

	vec3 diffuse = (vec3(1) - light.intensity) * diffusePower;
	vec3 specular = light.intensity * specularPower;

	float attenuation = 1 / dot(toLight, toLight);

	return (diffuse + specular) * attenuation; 
}

void main()
{
	vec3 norm = normalize(Normal);
	vec3 cameraToFrag = cameraPosition - FragPos;
	ssao = vec4(norm, length(cameraToFrag));

	vec3 viewDirection = normalize(cameraToFrag);

	vec3 result = vec3(0.1,0.1,0.1);
	for(int i = 0; i < MAX_LIGHTS; i++)
	{
		result += pointLightContribution(pointLights[i], norm, FragPos, viewDirection);
	}

	colour = vec4(result , 1.0); 
}