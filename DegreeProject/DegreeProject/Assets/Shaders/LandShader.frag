#version 120

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{ 
	vec4 pixel = texture2D(u_Texture, gl_TexCoord[0].xy);
	vec4 light = u_Color;

	vec4 col = pixel * u_Color;
	gl_FragColor = vec4(col);
}