struct PatchTess
{
	float EdgeTess[3] : SV_TessFactor;
	float InsideTess : SV_InsideTessFactor;
};

struct VertexOut
{
	float4 pos			: SV_POSITION;
	float2 texCoord		: TEXCOORD;
};


PatchTess PatchHS(InputPatch<VertexOut, 3> patch,
	uint patchID : SV_PrimitiveID)
{
	PatchTess pt;

	pt.EdgeTess[0] = 113.0f;
	pt.EdgeTess[1] = 113.0f;
	pt.EdgeTess[2] = 113.0f;
	pt.InsideTess  = 113.0f;

	return pt;
}

struct HullOut
{
	float4 pos			: SV_POSITION;
	float2 texCoord		: TEXCOORD;
};

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchHS")]
//[maxtessfactor(7.0)]
HullOut main(InputPatch<VertexOut, 3> p,
	uint i : SV_OutputControlPointID,
	uint patchId : SV_PrimitiveID)
{
	HullOut hout;

	// Pass through shader.
	hout.pos = p[i].pos;
	hout.texCoord = p[i].texCoord;

	return hout;
}