#pragma once

#ifndef HW_I_HARDWARE_BUFFER_MANAGER_H
#define HW_I_HARDWARE_BUFFER_MANAGER_H

#include "Base/HardwareBuffer.h"
#include "Hollow/Core/CModule.h"
#include "Hollow/Graphics/Vertex.h"

namespace Hollow {
	class HardwareBufferManager : public CModule<HardwareBufferManager>
	{
	public:
		virtual HardwareBuffer* create(const HARDWARE_BUFFER_DESC& desc) = 0;
	};
}

#endif