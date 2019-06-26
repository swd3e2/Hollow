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
TextureCube enivironmentMap : TEXTUTRE: register(t4);

SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap : register(s1);

float4 main(PixelShaderInput input) : SV_TARGET
{
	float4 result = float4(0.3f, 0.3f , 0.3f ,1.0f) + dot(input.normal, float3(1.0f, 0.0f, 0.0f)) * float4(0.2f, 0.3f, 0.3f, 0.0f);

	float3 ProjCoords = input.shadowPos.xyz / input.shadowPos.w;
	float2 uv;
	uv.x = 0.5f * ProjCoords.x + 0.5f;
	uv.y = 0.5f * -ProjCoords.y + 0.5f;
	float depth = ProjCoords.z;

	float shadowColor = shadow_map.Sample(SampleTypeWrap, uv).r;

	if (shadowColor < (depth - 0.00001f)) {
		result = -0.2f;
	}
	//float4 texcolor = ambient_map.Sample(SampleTypeWrap, input.texCoord);
	//if (texcolor.a < 0.2f) {
	//	//discard;
	//}

	return result;
}