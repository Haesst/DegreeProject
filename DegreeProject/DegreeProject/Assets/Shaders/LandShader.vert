#version 330 core

layout(location = 0)in vec3 a_Position;
layout(location = 1)in vec3 a_Normal;
layout(location = 2)in vec2 a_TexCoord;

out vec2 f_TexCoord;

void main()
{
	gl_Position = vec4(1.0, 1.0, 0.0, 1.0);
}