#pragma once 

#ifndef HW_SAMPLER_H
#define HW_SAMPLER_H

#include "CommonTypes.h"

namespace Hollow {
	struct SAMPLER_STATE_DESC
	{
		AddressingMode addressingMode	= AddressingMode::WRAP;
		FilterMode minFilterModel		= FilterMode::LINEAR;
		FilterMode magFilterMode		= FilterMode::LINEAR;
		int maxAnisotropy				= 1;
		CompareFunction compareFunction = CompareFunction::NNEVER;
	};

	class SamplerState
	{

	};
}

#endif