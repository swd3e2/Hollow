struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

Texture2D objTexture : TEXUTRE : register(t0);
SamplerState SampleTypeClamp : register(s0);

float4 PSMain(PixelShaderInput input) : SV_TARGET
{
    return objTexture.Sample(SampleTypeClamp, input.texCoord);
}