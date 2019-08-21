#pragma once

namespace Hollow {
	enum CompareFunction 
	{
		NNEVER,
		NLESS,
		NEQUAL,
		NLEQUAL,
		NGREATER,
		NNOT_EQUAL,
		NALWAYS
	};

	enum AddressingMode
	{
		WRAP,
		CLAMP,
		MIRROR,
		BORDER
	};

	enum FilterMode
	{
		NONE,
		POINT,
		LINEAR,
		ANISOTROPIC
	};
}