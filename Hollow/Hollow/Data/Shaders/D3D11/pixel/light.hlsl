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
	normal = normal * 2.0f - 1.0f;
	float4 diffuse = diffuseBuffer.Sample(SampleTypeClamp, input.texCoord);

	float4 shadowPos = mul(float4(position, 1.0f), ShadowWVP);
	float3 ProjCoords = shadowPos.xyz / shadowPos.w;
	float2 uv;
	uv.x = 0.5f * ProjCoords.x + 0.5f;
	uv.y = 0.5f * -ProjCoords.y + 0.5f;
	float depth = ProjCoords.z;
	
	float2 shadowMapOffset = (1.0 / 1920.0f, 1.0f / 1080.0f);

	float shadowFactor = 0.0f;

	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= -1; y++) {
			float2 offsets = float2(x, y);
			float shadowColor = shadow_map.Sample(SampleTypeClamp, (uv + offsets * shadowMapOffset)).r;
			shadowFactor += shadowColor > (depth - 0.000001f) ? 1.0f : 0.0f;
		}
	}

	shadowFactor /= 9.0f;

	diffuse -= shadowFactor * -1.0f;

	return diffuse + float4(0.00f, 0.01f, 0.03f,1.0f) * dot(normal, float3(0.0f, 1.0f, 0.0f));
}