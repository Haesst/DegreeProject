#version 120

uniform vec4 u_Color;
uniform vec4 u_OccupiedColor;
uniform sampler2D u_Texture;
uniform bool u_Highlighted;

void main()
{
	int frequency = 350;
	vec4 highlightColor = vec4(1.4, 1.4, 1.4, 1.0);

	//vec4 occupiedColor = vec4(1.0, 0.2, 0.4, 1.0);
	vec2 uv = gl_TexCoord[0].xy;
	float stripes = mix(uv.x, uv.y, 0) * frequency;//frequency * uv.x;
	float rounded = floor(stripes + 0.5);

	vec4 pixel = texture2D(u_Texture, gl_TexCoord[0].xy);
	vec4 light = u_Color;

	vec4 col = pixel;
	if (mod(rounded, 2.0) == 0.0)
	{
		col = col * u_Color;
	}
	else
	{
		col = col * u_OccupiedColor;
	}

	if (u_Highlighted)
	{
		col = col * highlightColor;
	}

	gl_FragColor = vec4(col);
}