struct DomainOut
{
	float4 pos     : SV_POSITION;
	float2 texCoord      : TEXCOORD;
};

struct PatchTess
{
	float EdgeTess[3] : SV_TessFactor;
	float InsideTess : SV_InsideTessFactor;
};

struct HullOut
{
	float4 pos			: SV_POSITION;
	float2 texCoord		: TEXCOORD;
};

cbuffer ConstantBuffer : register(b0)
{
	matrix WVP;
	float gMaxTessDistance;
	float gMinTessDistance;
	float gMinTessFactor;
	float gMaxTessFactor;
}

cbuffer ConstantBuffer : register(b1)
{
	matrix World;
	matrix View;
	matrix Projection;
	float3 cameraPosition;
	float offset;
}

Texture2D normal_map    : TEXTUTRE: register(t0);
SamplerState SamplerTypeClamp : register(s0);
SamplerState SamplerTypeWrap : register(s1);

// The domain shader is called for every vertex created by the tessellator.  
// It is like the vertex shader after tessellation.
[domain("tri")]
DomainOut main(PatchTess patchTess,
	float3 BarycentricCoordinates : SV_DomainLocation,
	const OutputPatch<HullOut, 3> TrianglePatch)
{
	DomainOut dout;

	float3 vWorldPos =
		BarycentricCoordinates.x * TrianglePatch[0].pos +
		BarycentricCoordinates.y * TrianglePatch[1].pos +
		BarycentricCoordinates.z * TrianglePatch[2].pos;

	dout.texCoord = BarycentricCoordinates.x * TrianglePatch[0].texCoord + BarycentricCoordinates.y * TrianglePatch[1].texCoord + BarycentricCoordinates.z * TrianglePatch[2].texCoord;
	dout.texCoord += offset;
	float fDisplacement = normal_map.SampleLevel(SamplerTypeWrap, dout.texCoord, 0).r;
	vWorldPos += float4(0.0f, -1.0f, 0.0f, 0.0f) * fDisplacement;
	dout.pos = mul(float4( vWorldPos.xyz, 1.0f ), WVP);

	return dout;
}