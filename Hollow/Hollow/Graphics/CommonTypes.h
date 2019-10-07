#pragma once

namespace Hollow {
	enum ComparisonFunction
	{
		CMP_NEVER,
		CMP_LESS,
		CMP_EQUAL,
		CMP_LEQUAL,
		CMP_GREATER,
		CMP_NOT_EQUAL,
		CMP_ALWAYS
	};

	enum AddressingMode
	{
		AM_WRAP,
		AM_CLAMP,
		AM_MIRROR,
		AM_BORDER
	};

	enum FilterMode
	{
		FM_NONE,
		FM_POINT,
		FM_LINEAR,
		FM_ANISOTROPIC
	};

	enum CullMode
	{
		CLM_NONE,
		CLM_FRONT,
		CLM_BACK
	};

	enum RENDER_TARGET_TEXTURE_FORMAT
	{
		R8G8B8A8,
		R32G32B32A32
	};

	enum StencilOperation
	{
		SOP_KEEP,
		SOP_ZERO,
		SOP_INCR,
		SOP_DECR,
		SOP_INCR_WRAP,
		SOP_DECR_WRAP,
		SOP_INVERT,
		SOP_REPLACE
	};

	enum DepthWriteMask
	{
		DWM_ZERO,
		DWM_ALL
	};

	enum FillMode
	{
		FM_SOLID,
		FM_WIREFRAME
	};

	enum BlendFunction
	{
		BF_ZERO,
		BF_ONE,
		BF_SRC_COLOR,
		BF_INV_SRC_COLOR,
		BF_SRC_APLHA,
		BF_INV_SRC_APLHA,
		BF_DEST_APLHA,
		BF_INV_DEST_APLHA,
		BF_DEST_COLOR,
		BF_INV_DEST_COLOR,
		BF_BLEND_FACTOR,
		BF_INV_BLEND_FACTOR
	};

	enum BlendOperation
	{
		BOP_ADD,
		BOP_SUBTRACT,
		BOP_REV_SUBTRACT,
		BOP_MIN,
		BOP_MAX
	};

	enum TextureFormat
	{
		TF_B8G8R8A8_UNORM,
		TF_R32G32B32A32
	};

	enum TextureType
	{
		TT_TEXTURE1D,
		TT_TEXTURE2D,
		TT_TEXTURE3D,
		TT_TEXTURE_CUBE
	};

	enum ShaderType
	{
		ST_VERTEX, 
		ST_PIXEL, 
		ST_GEOMERTY, 
		ST_COMPUTE, 
		ST_HULL, 
		ST_DOMAIN
	};
}