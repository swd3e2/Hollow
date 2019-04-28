struct PixelShaderInput
{
    float4 pos : SV_POSITION;
};

float4 PSMain(PixelShaderInput input) : SV_TARGET
{   
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}