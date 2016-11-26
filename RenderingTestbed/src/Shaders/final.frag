#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;

float ss(float coord)
{
	return 0.5 + (sin(250 * coord) * 0.5);
}


void main()
{ 

/*
	vec4 c = texture(screenTexture, TexCoords);
	if(TexCoords.x < 0.5)
	{
		c = vec4(1,1,1,1) - c;
	}

	if(TexCoords.y < 0.5)
	{
		c = vec4(1,1,1,1) - c;
	}

	color = c;
	*/

	/*
	vec4 texColour = texture(screenTexture, TexCoords);
	if(texColour != vec4(0,0,0,1))
	{
		//color = mix(texColour, vec4(1,1,1,1) - texColour, ss(TexCoords.x) * ss(TexCoords.y));
		color = mix(texColour, vec4(1, 0, 0 ,1), ss(TexCoords.x) * ss(TexCoords.y));
	}
	else
	{
		color = texColour;
	}
	*/
	
	vec4 c = texture(screenTexture, TexCoords);
	if((mod(TexCoords.x, 0.03) < 0.001 || mod(TexCoords.y, 0.03) < 0.001) && c == vec4(0,0,0,1))
	{
		c = vec4(0,0,0.5,1);
	}
	else if((mod(TexCoords.x, 0.01) < 0.001 || mod(TexCoords.y, 0.01) < 0.001) && c != vec4(0,0,0,1))
	{
		c = mix(vec4(1,1,1,1) - c, c, 0.85);
		//c = vec4(1,1,1,1) - c;
	}

	color = c;
}