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
    float4 texCoord : NORMAL0;
    float3 normal : TEXCOORD0;
};

struct VertexShaderInput
{
    float3 pos : POSITION;
    float4 texCoord : NORMAL0;
    float3 normal : TEXCOORD0;
};

PixelShaderOutput VSMain(VertexShaderInput input)
{
    PixelShaderOutput vertexShaderOutput;

    vertexShaderOutput.pos = float4(input.pos, 0.0f);
    vertexShaderOutput.pos = mul(vertexShaderOutput.pos, WVP);
    vertexShaderOutput.normal = input.normal;
    vertexShaderOutput.texCoord = input.texCoord;

    return vertexShaderOutput;
}