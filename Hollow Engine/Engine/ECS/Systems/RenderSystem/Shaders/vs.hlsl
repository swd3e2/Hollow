cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
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

    vertexShaderOutput.pos = mul(input.pos, World);
    vertexShaderOutput.pos = mul(vertexShaderOutput.pos, View);
    vertexShaderOutput.pos = mul(vertexShaderOutput.pos, Projection);
    vertexShaderOutput.col = input.col;
    return vertexShaderOutput;
}