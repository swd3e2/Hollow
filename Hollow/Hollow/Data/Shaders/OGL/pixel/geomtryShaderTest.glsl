#version 460

layout(location = 0) out vec4 diffuse;

in VS_OUT
{
	vec4 color;
} fs_in;

void main()
{
	diffuse = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}