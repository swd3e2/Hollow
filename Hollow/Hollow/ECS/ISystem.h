#pragma once

#ifndef HW_I_SYSTEM_H
#define HW_I_SYSTEM_H

#include "Hollow/Platform.h"
#include <limits>
#undef max
#undef min

namespace Hollow {
	using SystemTypeID = size_t;
	static const SystemTypeID INVALID_SYSTEMID = std::numeric_limits<SystemTypeID>::max();

	class ISystem
	{
	private:
		/// Summary:	Duration since last system update in milliseconds.
		float_t						m_TimeSinceLastUpdate;
		/// Summary:	The system update interval.
		/// A negative value means system should update each time the engine receives an update.
		float_t						m_UpdateInterval;
	public:
		ISystem()
		{}
		~ISystem()
		{}

		virtual inline const SystemTypeID GetStaticSystemTypeID() const = 0;
		virtual inline const char* GetSystemTypeName() const = 0;

		virtual void PreUpdate(double dt) = 0;
		virtual void Update(double dt) = 0;
		virtual void PostUpdate(double dt) = 0;
	};
}

#endif