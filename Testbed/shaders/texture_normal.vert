#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in mat4 transform;

layout(std140) uniform camera {
	mat4 projection;
	mat4 view;
	vec3 position;
} Camera;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

void main()
{
	gl_Position = Camera.projection * Camera.view * transform * vec4(position, 1.0f);
    TexCoords = texCoords;
	FragPos = vec3(transform * vec4(position, 1.0f));
	Normal = mat3(transpose(inverse(transform))) * normal;
} 