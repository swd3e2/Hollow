struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
};

cbuffer ConstantBuffer : register(b2)
{
    matrix transform;
    float id;
    bool selected;
}

Texture2D objTexture    : TEXUTRE : register(t0);
SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap : register(s1);

float4 PSMain(PixelShaderInput input) : SV_TARGET
{
    float4 color = objTexture.Sample(SampleTypeClamp, input.texCoord);
    if (selected) {
        color.g += 0.4f;
    }
    return color;
}