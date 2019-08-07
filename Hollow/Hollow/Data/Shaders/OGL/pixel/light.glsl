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
	vec2 texelSize;
	float bias;
};

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;

float SampleShadowMap(sampler2D shadowMap, vec2 uv, float compare)
{
	return step(compare, texture(shadowMap, uv).r);
}

float SampleShadowMapLinear(sampler2D shadowMap, vec4 shadowMapCoords)
{
	vec3 ProjCoords = shadowMapCoords.xyz / shadowMapCoords.w;
	vec2 uv;
	uv.x = 0.5f * ProjCoords.x + 0.5f;
	uv.y = 0.5f * ProjCoords.y + 0.5f;
	float z = 0.5f * ProjCoords.z + 0.5f;

	vec2 pixelPos = uv / texelSize + vec2(0.5);
	vec2 fracPart = fract(pixelPos);
	vec2 startTexel = (pixelPos - fracPart) * texelSize;

	float blTexel = SampleShadowMap(shadowMap, startTexel, z);
	float brTexel = SampleShadowMap(shadowMap, startTexel + vec2(texelSize.x, 0.0f), z);
	float tlTexel = SampleShadowMap(shadowMap, startTexel + vec2(0.0f, texelSize.y), z);
	float trTexel = SampleShadowMap(shadowMap, startTexel + vec2(texelSize.x, texelSize.y), z);

	float mixA = mix(blTexel, tlTexel, fracPart.y);
	float mixB = mix(brTexel, trTexel, fracPart.y);

	return mix(mixA, mixB, fracPart.x);
	/*if (clamp(uv.x, 0.0f, 1.0f) == uv.x && uv.y == clamp(uv.y, 0.0f, 1.0f)) {
		float shadowDepth = texture(shadowMap, uv).r;
		if (shadowDepth < (z - bias)) {

		}
	}*/
}

void main()
{
	vec2 texCoords = vec2(fs_in.texCoord.x, 1.0f - fs_in.texCoord.y);

	vec3 position = texture(tex2, texCoords).rgb;
	vec3 normal = texture(tex1, texCoords).rgb;
	vec4 diffuse = texture(tex0, texCoords);

	vec4 shadowPos = vec4(position, 1.0f) * ShadowWVP;
	SampleShadowMapLinear(tex3, shadowPos);

	FragColor = diffuse;
}