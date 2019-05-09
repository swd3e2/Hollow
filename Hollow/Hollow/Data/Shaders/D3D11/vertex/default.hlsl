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
}

cbuffer ConstantBuffer : register(b2)
{
	matrix transform;
}

struct AmbientLight
{
	float3 direction;
	float pad;
	float4 ambient;
};

struct PointLight
{
	float3 position;
	float range;
	float3 color;
	float pad;
	float3 attenuation;
};

cbuffer LightBuffer : register(b3)
{
	PointLight pointLight;
}

cbuffer ConstantBuffer : register(b5)
{
	matrix lightViewMatrix;
	matrix lightProjectionMatrix;
	float3 lightPosition;
}

struct PixelShaderOutput
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL0;
};

struct VertexShaderInput
{
	float3 pos : POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
};

PixelShaderOutput main(VertexShaderInput input)
{
	PixelShaderOutput output;

	output.pos = float4(input.pos, 1.0f);

	output.pos = mul(output.pos, transform);
	output.pos = mul(output.pos, WVP);
	output.normal = input.normal;
	output.texCoord = input.texCoord;

	return output;
}