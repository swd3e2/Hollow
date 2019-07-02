#version 460

out vec4 FragColor;

in VS_OUT
{
	vec2 texCoord;
} fs_in;

layout(std140, binding = 2) uniform PerObject
{
	mat4 transform;
	vec3 color;
	bool hasAnimation;
};

uniform sampler2D ambient_map;
uniform sampler2D normal_map;
uniform sampler2D specular_map;

void main()
{
	vec2 texCoords = vec2(fs_in.texCoord.x, 1.0f - fs_in.texCoord.y);

	vec3 position = texture(specular_map, texCoords).rgb;
	vec3 normal = texture(normal_map, texCoords).rgb;
	vec4 diffuse = texture(ambient_map, texCoords);

	FragColor = diffuse + clamp(dot(normal, vec3(0.0f, 1.0f, 0.0f)), 0.0f, 1.0f) * vec4(0.5f, 0.5f, 0.5f,1.0f);
	//FragColor = vec4(normal, 1.0f);
}