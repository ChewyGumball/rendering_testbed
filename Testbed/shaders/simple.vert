#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in mat4 transform;

layout(std140) uniform camera {
	mat4 projection;
	mat4 view;
	vec3 position;
} Camera;


void main()
{
	gl_Position = Camera.projection * Camera.view * transform * vec4(position, 1.0f);
	//gl_Position = vec4(position, 1.0f);
}