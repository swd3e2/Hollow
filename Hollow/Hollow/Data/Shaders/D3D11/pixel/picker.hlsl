struct PixelShaderInput
{
	float4 pos : SV_POSITION;
};

cbuffer ConstantBuffer : register(b2)
{
	matrix transform;
	float3 color;
	bool hasAnimation;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	
	return float4(color, 1.0f);
}