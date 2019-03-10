struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
};

cbuffer ConstantBuffer : register(b2)
{
    matrix transform;
}

cbuffer LightBuffer : register(b3)
{
    float3 direction;
    float pad;
    float4 ambient;
}

Texture2D objTexture    : TEXUTRE : register(t0);
SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap : register(s1);

float4 addAmbientColor(float3 normal)
{
    return saturate(dot(normal, direction) * ambient);
}

float4 PSMain(PixelShaderInput input) : SV_TARGET
{
    float4 color = objTexture.Sample(SampleTypeClamp, input.texCoord);

    color += addAmbientColor(input.normal);

    return color;
}