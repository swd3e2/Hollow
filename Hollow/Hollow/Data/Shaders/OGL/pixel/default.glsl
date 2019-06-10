#version 460

out vec4 FragColor;

in VS_OUT
{
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
} fs_in;

uniform sampler2D ambient_map;
uniform sampler2D normal_map;
uniform sampler2D specular_map;
uniform sampler2D shadow_map;

void main()
{
	FragColor = texture(ambient_map, fs_in.texCoord);
	FragColor += dot(fs_in.normal, vec3(-1.0f, 0.0f, 0.0f)) * vec4(0.5f, 0.5f, 0.5f, 1.0f);
}