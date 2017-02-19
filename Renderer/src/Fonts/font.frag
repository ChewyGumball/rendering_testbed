#version 330 core
in vec2 TexCoords;
in vec4 LetterColour;
out vec4 color;

uniform sampler2D font;

void main()
{
	color = texture(font, TexCoords).r * LetterColour;
}