struct PixelShaderOutput
{
	float4 pos		: SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

Texture2D diffuseBuffer : TEXTUTRE: register(t0);
Texture2D normalBuffer : TEXTUTRE: register(t1);
Texture2D positionBuffer : TEXTUTRE: register(t2);

SamplerState SampleTypeClamp : register(s0);

float4 main(PixelShaderOutput input) : SV_TARGET
{
	float3 position = positionBuffer.Sample(SampleTypeClamp, input.texCoord).rgb;
	float3 normal = normalBuffer.Sample(SampleTypeClamp, input.texCoord).rgb;
	float4 diffuse = diffuseBuffer.Sample(SampleTypeClamp, input.texCoord);



	return diffuse + float4(0.5f, 0.5f, 0.5f,1.0f) * dot(normal, float3(0.0f, 1.0f, 0.0f));
}