#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in mat4 transform;

uniform mat4 projection;
uniform mat4 view;

out vec3 FragPos;
out vec3 Normal;

void main()
{
	gl_Position = projection * view * transform * vec4(position, 1.0f);
	FragPos = vec3(transform * vec4(position, 1.0f));
	Normal = mat3(transpose(inverse(transform))) * normal;
	//Normal = normal;
	//testing file size change
}