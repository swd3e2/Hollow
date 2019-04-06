struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float4 worldPos : WORLD_POS;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float4 lightViewPosition : TEXCOORD1;
    float3 lightPos : TEXCOORD2;
};



Texture2D ambient_map : TEXUTRE : register(t0);

SamplerState SampleTypeClamp : register(s0);

float4 PSMain(PixelShaderInput input) : SV_TARGET
{
    return ambient_map.Sample(SampleTypeClamp, input.texCoord);
}