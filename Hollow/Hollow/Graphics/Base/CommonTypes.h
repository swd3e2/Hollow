#pragma once

namespace Hollow {
	enum ComparisonFunction 
	{
		NEVER,
		LESS,
		EQUAL,
		LEQUAL,
		GREATER,
		NOT_EQUAL,
		ALWAYS
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