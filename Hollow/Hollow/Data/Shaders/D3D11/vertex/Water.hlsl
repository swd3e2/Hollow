cbuffer ConstantBuffer : register(b0)
{
	matrix WVP;
}

cbuffer ConstantBuffer : register(b1)
{
	matrix World;
	matrix View;
	matrix Projection;
	float3 cameraPosition;
	float offset;
}

cbuffer ConstantBuffer : register(b2)
{
	matrix transform;
}

struct VertexShaderOutput
{
	float4 pos			: SV_POSITION;
	float2 texCoord		: TEXCOORD;
};

struct VertexShaderInput
{
	float3 pos		: POSITION;
	float2 texCoord : TEXCOORD;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;

	output.pos = float4(input.pos, 1.0f);
	output.texCoord = input.texCoord;

	return output;
}