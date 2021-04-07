#version 120

uniform mat4 scale;

void main()
{
    vec4 vertex = gl_Vertex;

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_FrontColor = gl_Color;
}