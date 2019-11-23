#version 330 core
layout(points) in;
layout(line_strip, max_vertices = 2) out;

in gl_PerVertex
{
	vec4  gl_Position;
	float gl_PointSize;
	float gl_ClipDistance[];
} gl_in[];

in VS_OUT{
   vec4 color;
} gs_cin[];

out GS_OUT{
   vec4 color;
} gs_out;

void main()
{
	gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);
	gs_out.color = gs_cin[0].color;
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4(0.1, 0.0, 0.0, 0.0);
	gs_out.color = gs_cin[0].color;
	EmitVertex();

	EndPrimitive();
}