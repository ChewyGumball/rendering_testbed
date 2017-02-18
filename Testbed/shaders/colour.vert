#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in mat4 transform;

uniform mat4 projection;
uniform mat4 view;

out vec4 vertexColour;

void main()
{
	gl_Position = projection * view * transform * vec4(position, 1.0f);
	//gl_Position = transform * vec4(position, 1.0f);
	vertexColour = colour;
}