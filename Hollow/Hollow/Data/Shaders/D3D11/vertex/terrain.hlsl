cbuffer ConstantBuffer : register(b0)
{
	matrix WVP;
	float3 cameraPosition;
};

cbuffer ConstantBuffer : register(b2)
{
	matrix transform;
	float3 color;
	bool hasAnimation;
};

cbuffer ConstantBuffer : register(b7)
{
	matrix boneInfo[100];
};

struct PixelShaderOutput
{
	float4 hPos		: HPOS;
	float4 position	: SV_POSITION;
	float3 normal	: NORMAL;
	float2 texCoord	: TEXCOORD0;
};

struct VertexShaderInput
{
	float3 pos		: POSITION;
	float2 texCoord	: TEXCOORD;
	float3 normal	: NORMAL;
};

PixelShaderOutput main(VertexShaderInput input)
{
	PixelShaderOutput output;
	output.position = mul(float4(input.pos, 1.0f), transform);
	output.hPos = output.position;

	output.position = mul(output.position, WVP);
	output.normal = normalize(mul(input.normal, transform)) + 1.0f * 0.5f;
	output.texCoord = input.texCoord;

	return output;
}