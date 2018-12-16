#ifndef __I_SYSTEM_H__
#define __I_SYSTEM_H__
#pragma once
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

		unsigned char						m_Enabled : 1;
		unsigned char						m_NeedsUpdate : 1;
		unsigned char						m_Reserved : 6;

	protected:
	
	public:
		ISystem()
		{}
		~ISystem()
		{}

		virtual inline const SystemTypeID GetStaticSystemTypeID() const = 0;
		virtual inline const char* GetSystemTypeName() const = 0;

		virtual void PreUpdate(float_t dt) = 0;
		virtual void Update(float_t dt) = 0;
		virtual void PostUpdate(float_t dt) = 0;
	};

}

#endif