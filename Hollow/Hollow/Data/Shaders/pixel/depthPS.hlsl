struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float4 depthPosition : DEPTH_POSITION;
};

float4 PSMain(PixelShaderInput input) : SV_TARGET
{   
    float depthValue = input.depthPosition.z / input.depthPosition.w;
    return float4(depthValue, depthValue, depthValue, 1.0f);
}