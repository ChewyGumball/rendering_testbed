#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in mat4 transform;

uniform mat4 projection;
uniform mat4 view;

out vec2 TexCoords;

void main()
{
	gl_Position = projection * view * transform * vec4(position, 1.0f);
    TexCoords = texCoords;
} 