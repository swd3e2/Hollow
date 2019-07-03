struct PixelShaderOutput
{
	float4 pos		: SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

cbuffer ConstantBuffer : register(b1)
{
	matrix ShadowWVP;
}

Texture2D diffuseBuffer : TEXTUTRE: register(t0);
Texture2D normalBuffer : TEXTUTRE: register(t1);
Texture2D positionBuffer : TEXTUTRE: register(t2);
Texture2D shadow_map : TEXTUTRE: register(t3);

SamplerState SampleTypeClamp : register(s0);

float4 main(PixelShaderOutput input) : SV_TARGET
{
	float3 position = positionBuffer.Sample(SampleTypeClamp, input.texCoord).rgb;
	float3 normal = normalBuffer.Sample(SampleTypeClamp, input.texCoord).rgb;
	float4 diffuse = diffuseBuffer.Sample(SampleTypeClamp, input.texCoord);

	float4 shadowPos = mul(float4(position, 1.0f), ShadowWVP);
	float3 ProjCoords = shadowPos.xyz / shadowPos.w;
	float2 uv;
	uv.x = 0.5f * ProjCoords.x + 0.5f;
	uv.y = 0.5f * -ProjCoords.y + 0.5f;
	float depth = ProjCoords.z;

	if (clamp(uv.x, 0.0f, 1.0f) == uv.x && clamp(uv.y, 0.0f, 1.0f) == uv.y) {
		float shadowColor = shadow_map.Sample(SampleTypeClamp, uv).r;
		if (shadowColor < (depth - 0.000001f)) {
			diffuse -= 0.3f;
		}
	}

	return diffuse + float4(0.5f, 0.5f, 0.5f,1.0f) * dot(normal, float3(0.0f, 1.0f, 0.0f));
}