#version 460

in VS_OUT
{
	vec3 texCoord;
} fs_in;

out vec4 FragColor;

uniform samplerCube environmentMap;

void main()
{
	FragColor = texture(environmentMap, fs_in.texCoord);
}