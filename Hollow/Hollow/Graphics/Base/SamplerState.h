#pragma once 

#ifndef HW_SAMPLER_H
#define HW_SAMPLER_H

#include "CommonTypes.h"
#define MAX_LOD ( 3.402823466e+38f )

namespace Hollow {
	struct SAMPLER_STATE_DESC
	{
		AddressingMode addressingMode			= AddressingMode::WRAP;
		FilterMode minFilterModel				= FilterMode::LINEAR;
		FilterMode magFilterMode				= FilterMode::LINEAR;
		FilterMode mipFilterMode				= FilterMode::NONE;
		int maxAnisotropy						= 1;
		ComparisonFunction comparisonFunction	= ComparisonFunction::NEVER;
		float minLOD							= 0;
		float maxLOD							= MAX_LOD;
		float mipLODBias						= 0;
	};

	class SamplerState
	{
	public:
		SamplerState* create(const SAMPLER_STATE_DESC* desc);
	};
}

#endif