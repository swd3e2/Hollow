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
    float4 col : COLOR0;
};

struct VertexShaderInput
{
    float4 pos : POSITION;
    float4 col : COLOR0;
};

PixelShaderOutput VSMain(VertexShaderInput input)
{
    PixelShaderOutput vertexShaderOutput;

    vertexShaderOutput.pos = mul(input.pos, transform);
    vertexShaderOutput.pos = mul(vertexShaderOutput.pos, WVP);
    vertexShaderOutput.col = input.col;
    return vertexShaderOutput;
}