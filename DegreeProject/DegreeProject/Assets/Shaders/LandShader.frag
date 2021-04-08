#version 120

uniform vec4 u_Color;
uniform vec4 u_OccupiedColor;
uniform sampler2D u_Texture;
uniform bool u_Highlighted;
uniform int u_Frequency = 150;
uniform vec4 u_HighlightColor = vec4(1.4, 1.4, 1.4, 1.0);

vec4 getColor(vec2 uv, int frequency)
{
	float pos = mix(uv.x, uv.y, 0.3) * frequency;
	float value = floor(fract(pos) + 0.5);
	return mix(u_Color, u_OccupiedColor, value);
}

void main()
{	
	vec4 color = getColor(gl_TexCoord[0].xy, u_Frequency);
	vec4 pixel = texture2D(u_Texture, gl_TexCoord[0].xy);
	
	vec4 col = pixel * color;

	if (u_Highlighted)
	{
		col = col * u_HighlightColor;
	}

	gl_FragColor = vec4(col);
}