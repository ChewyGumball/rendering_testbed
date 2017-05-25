#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;

void main()
{
	color = texture(screenTexture, TexCoords);
	if(color.a < 0.001) {
		discard;
	}
}