#version 120

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    vec2 texCoord = gl_TexCoord[0].xy;
	vec4 pixelColor = texture2D(u_Texture, texCoord);

	gl_FragColor = vec4(pixelColor * u_Color);
}