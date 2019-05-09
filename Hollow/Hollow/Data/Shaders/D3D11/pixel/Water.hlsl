struct DomainOut
{
	float4 pos     	: SV_POSITION;
	float2 texCoord : TEXCOORD;
};

cbuffer ConstantBuffer : register(b1)
{
	matrix World;
	matrix View;
	matrix Projection;
	float3 cameraPosition;
	float offset;
}

Texture2D normal_map    : TEXTUTRE: register(t0);
SamplerState SamplerTypeWrap : register(s1);

float4 main(DomainOut input) : SV_TARGET
{
	float displacment = normal_map.SampleLevel(SamplerTypeWrap, input.texCoord + offset, 0).r;

	return float4(displacment,0.0f,0.0f,0.0f);
}