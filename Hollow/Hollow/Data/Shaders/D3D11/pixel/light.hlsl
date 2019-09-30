struct PixelShaderOutput
{
	float4 pos		: SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

struct Light
{
	float3 position;		// 12
	float constant;		// 16
	float3 direction;		// 28
	float linearAttenuation;		// 32
	float3 color;			// 44
	float quadratic;	// 48
	float cutoff;		// 52
	float distance;		// 56
	int type;			// 60
	int pad;			// 64
};

cbuffer ConstantBuffer : register(b1)
{
	matrix ShadowWVP;
	float2 texelSize;
	float bias;
}

cbuffer ConstantBuffer : register(b5)
{
	Light lights[100];
	int numLights;
}

Texture2D diffuseBuffer : TEXTUTRE: register(t0);
Texture2D normalBuffer : TEXTUTRE: register(t1);
Texture2D positionBuffer : TEXTUTRE: register(t2);
Texture2D shadow_map : TEXTUTRE: register(t3);

SamplerState SampleTypeClamp : register(s0);

float calculateShadowAmount(float3 position)
{
	float4 shadowPos = mul(float4(position, 1.0f), ShadowWVP);
	float3 ProjCoords = shadowPos.xyz / shadowPos.w;
	float2 uv;
	uv.x = 0.5f * ProjCoords.x + 0.5f;
	uv.y = 0.5f * -ProjCoords.y + 0.5f;
	float depth = ProjCoords.z;

	float shadowFactor = 0.0f;
	
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= -1; y++) {
			float2 offsets = float2(x, y);
			float shadowColor = shadow_map.Sample(SampleTypeClamp, (uv + offsets * texelSize)).r;
			shadowFactor += shadowColor > (depth - bias) ? 1.0f : 0.0f;
		}
	}
	shadowFactor /= 9.0f;

	return shadowFactor;
}

float4 main(PixelShaderOutput input) : SV_TARGET
{
	float3 position = positionBuffer.Sample(SampleTypeClamp, input.texCoord).rgb;
	float3 normal = normalBuffer.Sample(SampleTypeClamp, input.texCoord).rgb;
	normal = normal * 2.0f - 1.0f;
	float4 diffuse = diffuseBuffer.Sample(SampleTypeClamp, input.texCoord);
	
	for (int i = 0; i < numLights; i++) {
		//diffuse += float4(lights[i].color, 1.0f); // CalcPointLight(lights[i], normal, position);
	}

	return diffuse;
}