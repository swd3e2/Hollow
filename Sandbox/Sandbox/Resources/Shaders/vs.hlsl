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
    float id;
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
};

PixelShaderOutput VSMain(VertexShaderInput input)
{
    PixelShaderOutput vertexShaderOutput;

    vertexShaderOutput.pos = float4(input.pos, 1.0f);
    vertexShaderOutput.pos = mul(vertexShaderOutput.pos, transform);
    vertexShaderOutput.pos = mul(vertexShaderOutput.pos, WVP);
    vertexShaderOutput.normal = input.normal;
    vertexShaderOutput.texCoord = input.texCoord;

    return vertexShaderOutput;
}