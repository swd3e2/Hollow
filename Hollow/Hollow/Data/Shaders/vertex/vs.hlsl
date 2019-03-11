cbuffer ConstantBuffer : register(b0)
{
    matrix WVP;
}

cbuffer ConstantBuffer : register(b1)
{
    matrix World;
    matrix View;
    matrix Projection;
}

cbuffer ConstantBuffer : register(b2)
{
    matrix transform;
}

struct PixelShaderOutput
{
    float4 pos : SV_POSITION;
    float4 worldPos : WORLD_POS;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};

struct VertexShaderInput
{
    float3 pos : POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};

PixelShaderOutput VSMain(VertexShaderInput input)
{
    PixelShaderOutput output;

    output.pos = float4(input.pos, 1.0f);
    output.pos = mul(output.pos, transform);
    output.worldPos = output.pos;

    output.pos = mul(output.pos, WVP);
    output.normal = mul(input.normal, transform);
    output.texCoord = input.texCoord;

    output.tangent = input.tangent;
    output.bitangent = input.bitangent;

    return output;
}