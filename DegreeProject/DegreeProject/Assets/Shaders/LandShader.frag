#version 120

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform bool u_Highlighted;

void main()
{
	vec4 highlightColor = vec4(1.0, 1.0, 1.0, 0.4);

	vec4 pixel = texture2D(u_Texture, gl_TexCoord[0].xy);
	vec4 light = u_Color;

	vec4 col = pixel * u_Color;

	if (u_Highlighted)
	{
		col = col + highlightColor;
	}

	gl_FragColor = vec4(col);
}