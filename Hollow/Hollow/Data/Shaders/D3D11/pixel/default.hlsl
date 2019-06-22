struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL0;
	float3 cubemapDirection : CUBEMAPCOORD;
};

cbuffer ConstantBuffer : register(b2)
{
	matrix transform;
	float3 color;
	float pad;
	int selected;
	int hasAnimation;
}

Texture2D ambient_map		: TEXTUTRE: register(t0);
Texture2D normal_map		: TEXTUTRE: register(t1);
Texture2D specular_map		: TEXTUTRE: register(t2);
TextureCube enivironmentMap : TEXTUTRE: register(t4);

SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap : register(s1);

float4 main(PixelShaderInput input) : SV_TARGET
{
	if (selected == 1) {
		return float4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	return float4(0.3f, 0.3f, 0.3f, 0.0f) + dot(input.normal, float3(1.0f, 0.0f, 0.0f)) * float4(0.4f, 0.7f, 0.7f, 0.0f);
	return ambient_map.Sample(SampleTypeWrap, input.texCoord);
}