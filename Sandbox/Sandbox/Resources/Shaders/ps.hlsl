struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
};

float4 PSMain(PixelShaderInput input) : SV_TARGET
{
    return input.col;
}