#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 letterColour;
layout (location = 2) in vec4 meshBounds;
layout (location = 3) in vec4 textureBounds;
layout (location = 4) in mat4 transform;

layout(std140) uniform camera {
	mat4 projection;
	mat4 view;
	vec3 position;
} Camera;

out vec2 TexCoords;
out vec4 LetterColour;

void main()
{
	gl_Position = Camera.projection * Camera.view * transform * vec4(mix(meshBounds.xy, meshBounds.zw, position.xy), 0, 1);
	TexCoords = mix(textureBounds.xy, textureBounds.zw, position.xy);
	LetterColour = letterColour;
} 