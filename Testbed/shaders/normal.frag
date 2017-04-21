#version 330 core

struct PointLight {
	vec3 position;
	vec3 intensity;
	float power;
};

layout(std140) uniform camera {
	mat4 projection;
	mat4 view;
	vec3 position;
} Camera;

const int MAX_LIGHTS = 2;
layout(std140) uniform lights {
	PointLight pointLights[MAX_LIGHTS];
} Lights;

in vec3 FragPos;
in vec3 Normal;

out vec4 colour;

vec3 pointLightContribution(PointLight light, vec3 normal, vec3 position, vec3 viewDirection)
{
	vec3 toLight = light.position - position;
	vec3 lightDirection = normalize(toLight);
	float diffusePower = max(dot(normal, lightDirection), 0.0);

	vec3 reflectedDirection = reflect(-lightDirection, normal);
	vec3 h = normalize(lightDirection + viewDirection);
	float ndoth = dot(normal, h);
	//float specularPower = pow(clamp(ndoth,0.0,1.0), 512);
	float specularPower = pow(max(dot(viewDirection, reflectedDirection), 0.0), 256);

	vec3 diffuse = (vec3(1) - light.intensity) * diffusePower;
	vec3 specular = light.intensity * specularPower;

	float attenuation = 1 / dot(toLight, toLight);

	return (diffuse + specular) * clamp(light.power * attenuation, 0, 1); 
}

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDirection = normalize(Camera.position - FragPos);

	vec3 result = vec3(0,0,0);
	for(int i = 0; i < MAX_LIGHTS; i++)
	{
		result += pointLightContribution(Lights.pointLights[i], norm, FragPos, viewDirection);
	}

	colour = vec4(result, 1.0); 
}