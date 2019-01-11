struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
};

cbuffer ConstantBuffer : register(b2)
{
    matrix transform;
    int kek;
}

Texture2D objTexture    : TEXUTRE : register(t0);
SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap : register(s1);

float4 PSMain(PixelShaderInput input) : SV_TARGET
{
    if (kek > 0) {
        return objTexture.Sample(SampleTypeClamp, input.texCoord);
    }
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}