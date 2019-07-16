cbuffer ConstantBuffer : register(b1)
{
	matrix shadowWVP;
}

cbuffer ConstantBuffer : register(b2)
{
    matrix transform;
}

struct VertexShaderOutput
{
    float4 pos : SV_POSITION;
	float4 spos : SHADOW_POSITION;
};

struct VertexShaderInput
{
	float3 pos			: POSITION;
	float2 texCoord		: TEXCOORD;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 bitangent	: BITANGENT;
};


VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.pos = float4(input.pos, 1.0f);
    output.pos = mul(output.pos, transform);
    output.pos = mul(output.pos, shadowWVP);
	output.spos = output.pos;

    return output;
}