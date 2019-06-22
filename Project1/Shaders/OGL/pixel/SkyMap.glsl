#version 460

in VS_OUT
{
	vec3 texCoord;
} fs_in;

out vec4 FragColor;

uniform samplerCube enivironmentMap;

void main()
{
	FragColor = texture(enivironmentMap, fs_in.texCoord);
}