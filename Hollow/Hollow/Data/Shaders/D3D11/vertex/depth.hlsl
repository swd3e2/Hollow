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

cbuffer ConstantBuffer : register(b5)
{
    matrix lightViewMatrix;
    matrix lightProjectionMatrix;
    float3 lightPosition;
}

struct VertexShaderOutput
{
    float4 pos : SV_POSITION;
};

struct VertexShaderInput
{
    float3 pos : POSITION;
};


VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.pos = float4(input.pos, 1.0f);
    output.pos = mul(output.pos, transform);
    output.pos = mul(output.pos, lightViewMatrix);
    output.pos = mul(output.pos, lightProjectionMatrix);

    return output;
}