#version 460

in VS_OUT
{
	vec3 texCoord;
} fs_in;

out vec4 FragColor;

uniform samplerCube ambient_map;
uniform samplerCube normal_map;
uniform samplerCube specular_map;
uniform samplerCube shadow_map;

void main()
{
	FragColor = texture(ambient_map, fs_in.texCoord);
}