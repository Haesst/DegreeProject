#version 120

uniform float u_Time;
uniform float u_Wave_Speed;
uniform float u_Zoom_Level;
uniform vec2 u_Resolution;
uniform vec4 u_Color;

void main()
{
    vec2 uv = gl_TexCoord[0].xy / u_Resolution.xy;

	vec4 texture_color = vec4(0.192156862745098, 0.6627450980392157, 0.9333333333333333, 1.0);

	vec4 k = vec4(u_Time)*u_Wave_Speed;
	k.xy = uv * u_Zoom_Level;
	float val1 = length(0.5-fract(k.xyw*=mat3(vec3(-2.0,-1.0,0.0), vec3(3.0,-1.0,1.0), vec3(1.0,-1.0,-1.0))*0.5));
	float val2 = length(0.5-fract(k.xyw*=mat3(vec3(-2.0,-1.0,0.0), vec3(3.0,-1.0,1.0), vec3(1.0,-1.0,-1.0))*0.2));
	float val3 = length(0.5-fract(k.xyw*=mat3(vec3(-2.0,-1.0,0.0), vec3(3.0,-1.0,1.0), vec3(1.0,-1.0,-1.0))*0.5));
	vec4 color = vec4 ( pow(min(min(val1,val2),val3), 7.0) * 3.0)+u_Color;
    gl_FragColor = color;
}