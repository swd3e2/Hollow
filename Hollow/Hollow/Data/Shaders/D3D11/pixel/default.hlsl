struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL0;
	float3 cubemapDirection : CUBEMAPCOORD;
	float4 shadowPos : SHADOWPOS;
};

cbuffer ConstantBuffer : register(b2)
{
	matrix transform;
	float3 color;
	float pad;
	int selected;
	int hasAnimation;
}

cbuffer ConstantBuffer : register(b4) {
	float4 baseColor;
	float metallicFactor;
	float roughnessFactor;
	float emmisiveFactor;
	float pad2;
	bool hasDiffuseTexture;
	bool hasNormalMap;
	bool hasSpecularMap;
};

Texture2D ambient_map		: TEXTUTRE: register(t0);
Texture2D normal_map		: TEXTUTRE: register(t1);
Texture2D specular_map		: TEXTUTRE: register(t2);
Texture2D shadow_map		: TEXTUTRE: register(t3);
Texture2D shadow_map2		: TEXTUTRE: register(t5);
TextureCube enivironmentMap : TEXTUTRE: register(t4);

SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap : register(s1);

float4 main(PixelShaderInput input) : SV_TARGET
{

	float3 ProjCoords = input.shadowPos.xyz / input.shadowPos.w;
	float2 uv;
	uv.x = 0.5f * ProjCoords.x + 0.5f;
	uv.y = 0.5f * -ProjCoords.y + 0.5f;
	float depth = ProjCoords.z;


	float4 result = ambient_map.Sample(SampleTypeClamp, input.texCoord);
	if (result.a < 0.05f) {
		//discard;
	}
	float shadowColor = shadow_map2.Sample(SampleTypeWrap, uv).r;

	if (saturate(uv.x) == uv.x && saturate(uv.y) == uv.y) {
		if (shadowColor < (depth - 0.000001f)) {
			result = ambient_map.Sample(SampleTypeClamp, input.texCoord) - 0.3f;
		}
	}
	

	return result;
}