struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
};

cbuffer ConstantBuffer : register(b2)
{
    int has_texture;
    matrix transform;
}

Texture2D objTexture    : TEXUTRE : register(t0);
SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap : register(s1);

float4 PSMain(PixelShaderInput input) : SV_TARGET
{
    if (has_texture == 1) {
        return objTexture.Sample(SampleTypeClamp, input.texCoord);
    }

   return float4(1.0f, 1.0f, 1.0f, 1.0f);
}