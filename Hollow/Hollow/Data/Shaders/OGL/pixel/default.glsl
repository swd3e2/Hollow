#version 460

out vec4 FragColor;

in VS_OUT
{
	vec4 shadowPos;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
	vec3 cubemapDirection;
} fs_in;

layout(std140, binding = 2) uniform PerObject
{
	mat4 transform;
	vec3 color;
	float pad2;
	bool selected;
	bool hasAnimation;
};

layout(std140, binding = 4) uniform MaterialData
{
	vec4 base_color;
	float metallicFactor;
	float roughnessFactor;
	float emmisiveFactor;
	float pad;
	bool hasDiffuseTexture;
	bool hasNormalMap;
	bool hasSpecularMap;
};

uniform sampler2D ambient_map;
uniform sampler2D normal_map;
uniform sampler2D specular_map;
uniform sampler2D shadow_map;
uniform sampler2D shadow_map2;
uniform samplerCube environmentMap;

void main()
{
	//FragColor = vec4(texture(shadow_map, fs_in.texCoord).r);
	//return;
	FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	vec3 ProjCoords = fs_in.shadowPos.xyz / fs_in.shadowPos.w;
	vec2 uv;
	uv.x = -0.5f + fs_in.shadowPos.x * 0.5f;
	uv.y = -0.5f + fs_in.shadowPos.y * 0.5f;
	float z = ProjCoords.z;


	if (clamp(uv.x, 0.0f, 1.0f) == uv.x && clamp(uv.y, 0.0f, 1.0f) == uv.y) {
		FragColor = vec4(uv.x, uv.y, 0.0f, 1.0f);

		/*float shadowColor = texture(shadow_map, uv).r;
		if (shadowColor < (z - 0.00001)) {
			FragColor -= 0.3f;
		}*/
	}
}