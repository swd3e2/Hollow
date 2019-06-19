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
	float4 pos : SV_POSITION;
};

struct VertexShaderInput
{
	float3 pos				: POSITION;
	float2 texCoord			: TEXCOORD;
	float3 normal			: NORMAL;
	float3 tangent			: TANGENT;
	float3 bitangent		: BITANGENT;
	int4   boneId			: BONEID;
	float4 weight			: WEIGHT;
};

PixelShaderOutput main(VertexShaderInput input)
{
	PixelShaderOutput output;
	output.pos = float4(input.pos, 1.0f);

	output.pos = mul(output.pos, transform);
	output.pos = mul(output.pos, WVP);

	return output;
}