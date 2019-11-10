cbuffer ConstantBuffer : register(b0)
{
	matrix WVP;
	float3 cameraPosition;
};

cbuffer ConstantBuffer : register(b2)
{
	matrix transform;
	float3 color;
	bool selected;
	bool hasAnimation;
};

cbuffer ConstantBuffer : register(b3)
{
    matrix worldTransform;
};

cbuffer ConstantBuffer : register(b6)
{
	matrix boneInfo[200];
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
	float3 pos			: POSITION;
	float2 texCoord		: TEXCOORD;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 bitangent	: BITANGENT;
	int4 boneId			: BONE;
	float4 weight		: WEIGHT;
};

PixelShaderOutput main(VertexShaderInput input)
{
	PixelShaderOutput output;

	output.position = float4(input.pos, 1.0f);
	output.normal = input.normal;
	
	if (hasAnimation) {
		matrix BoneTransform = boneInfo[input.boneId[0]] * input.weight[0];
		BoneTransform       += boneInfo[input.boneId[1]] * input.weight[1];
		BoneTransform       += boneInfo[input.boneId[2]] * input.weight[2];
		BoneTransform       += boneInfo[input.boneId[3]] * input.weight[3];

		output.position = mul(output.position, BoneTransform);
        output.normal = mul(output.normal, BoneTransform);
    } else {
        output.position = mul(output.position, worldTransform);
    }

    output.normal = normalize(mul(output.normal, transform)) + 1.0f * 0.5f;
	output.texCoord = input.texCoord;

    output.position = mul(output.position, transform);
    output.hPos = output.position;
    //output.hPos = mul(output.position, worldTransform);

    output.position = mul(output.hPos, WVP);

	return output;
}