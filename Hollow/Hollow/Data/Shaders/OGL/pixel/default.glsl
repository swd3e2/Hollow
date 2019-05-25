#version 460

out vec4 FragColor;

in VS_OUT
{
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
} fs_in;

void main()
{
	FragColor = vec4(fs_in.normal.x, fs_in.normal.y, fs_in.normal.z, 0.0f);

}