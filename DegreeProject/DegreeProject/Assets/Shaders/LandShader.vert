uniform float wave_phase;
uniform vec2 wave_amplitude;
uniform mat4 scale;

void main()
{
	float wave_phase = 1.0;
	vec2 wave_amplitude = vec2(1.0, 1.0);
    vec4 vertex = gl_Vertex;
	vertex = vertex;
	vec2 Scale = vec2(0.5,0.5);
	//vertex = vec4( vertex.x * Scale.x, vertex.y * Scale.y, 1, 1 );
	// mat4 mv = gl_ModelViewProjectionMatrix;
	// mat4 sm = mat4.scale(mv, [2,2,2]);
	// vertex.x = vertex.x * 0.8;
	// vertex.y = vertex.y * 0.8;
	// vertex.z = vertex.z * 0.8;
    // vertex.x += cos(gl_Vertex.y * 0.02 + wave_phase * 3.8) * wave_amplitude.x
    //           + sin(gl_Vertex.y * 0.02 + wave_phase * 6.3) * wave_amplitude.x * 0.3;
    // vertex.y += sin(gl_Vertex.x * 0.02 + wave_phase * 2.4) * wave_amplitude.y
    //           + cos(gl_Vertex.x * 0.02 + wave_phase * 5.2) * wave_amplitude.y * 0.3;

    gl_Position = gl_ModelViewProjectionMatrix * vertex;
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;
}