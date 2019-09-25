#pragma once 

#ifndef HW_SAMPLER_H
#define HW_SAMPLER_H

#include "CommonTypes.h"
#define MAX_LOD (3.402823466e+38f)

namespace Hollow {
	struct SAMPLER_STATE_DESC
	{
		AddressingMode addressingMode;
		FilterMode minFilterModel;
		FilterMode magFilterMode;
		FilterMode mipFilterMode;
		int maxAnisotropy;
		ComparisonFunction comparisonFunction;
		float minLOD;
		float maxLOD;
		float mipLODBias;

		SAMPLER_STATE_DESC() :
			addressingMode(AddressingMode::AM_WRAP), minFilterModel(FilterMode::FM_LINEAR), magFilterMode(FilterMode::FM_LINEAR), 
			mipFilterMode(FilterMode::FM_NONE), maxAnisotropy(1), comparisonFunction(ComparisonFunction::CMP_NEVER), minLOD(0), 
			maxLOD(MAX_LOD), mipLODBias(0)
		{}
	};

	class SamplerState
	{
	public:
		static SamplerState* create(const SAMPLER_STATE_DESC& desc);
	};
}

#endif