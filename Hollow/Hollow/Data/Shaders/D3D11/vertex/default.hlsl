cbuffer ConstantBuffer : register(b0)
{
	matrix WVP;
	float3 cameraPosition;
}

cbuffer ConstantBuffer : register(b1)
{
	matrix ShadowWVP;
}

cbuffer ConstantBuffer : register(b2)
{
	matrix transform;
	bool hasAnimation;
	bool selected;
}

cbuffer ConstantBuffer : register(b7)
{
	matrix boneInfo[100];
}

struct PixelShaderOutput
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL0;
	float3 cubemapDirection : CUBEMAPCOORD;
	float4 shadowPos : SHADOWPOS;
};

struct VertexShaderInput
{
	float3 pos		 : POSITION;
	float2 texCoord	 : TEXCOORD;
	float3 normal	 : NORMAL;
	float3 tangent	 : TANGENT;
	float3 bitangent : BITANGENT;
};

PixelShaderOutput main(VertexShaderInput input)
{
	PixelShaderOutput output;
	output.pos = float4(input.pos, 1.0f);

	output.normal = mul(input.normal, transform);

	output.pos = mul(output.pos, transform);
	output.shadowPos = mul(output.pos, ShadowWVP);

	float3 temp = cameraPosition - output.pos;
	output.cubemapDirection = reflect(temp, output.normal);

	output.pos = mul(output.pos, WVP);
	output.texCoord = input.texCoord;

	return output;
}