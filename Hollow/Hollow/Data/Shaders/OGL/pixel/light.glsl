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

layout(std140, binding = 1) uniform ShadowMatrices
{
	mat4 ShadowWVP;
};

uniform sampler2D ambient_map;
uniform sampler2D normal_map;
uniform sampler2D specular_map;
uniform sampler2D shadow_map;

void main()
{
	vec2 texCoords = vec2(fs_in.texCoord.x, 1.0f - fs_in.texCoord.y);

	vec3 position = texture(specular_map, texCoords).rgb;
	vec3 normal = texture(normal_map, texCoords).rgb;
	vec4 diffuse = texture(ambient_map, texCoords);

	vec4 shadowPos = vec4(position, 1.0f) * ShadowWVP;

	vec3 ProjCoords = shadowPos.xyz / shadowPos.w;
	vec2 uv;
	uv.x = 0.5f * ProjCoords.x + 0.5f;
	uv.y = 0.5f * ProjCoords.y + 0.5f;
	float z = 0.5f * ProjCoords.z + 0.5f;

	if (clamp(uv.x, 0.0f, 1.0f) == uv.x && uv.y == clamp(uv.y, 0.0f, 1.0f)) {
		float shadowDepth = texture(shadow_map, uv).r;
		if (shadowDepth < (z - 0.00001f)) {
			diffuse -= 0.3f;
		}
	}

	FragColor = diffuse + clamp(dot(normal, vec3(0.0f, 1.0f, 0.0f)), 0.0f, 1.0f) * vec4(0.5f, 0.5f, 0.5f,1.0f);
}