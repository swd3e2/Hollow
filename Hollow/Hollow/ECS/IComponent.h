#pragma once
#include "Hollow/Platform.h"

namespace Hollow {
	class ComponentManager;

	class HOLLOW_API IComponent
	{
		friend ComponentManager;
	protected:
		size_t ComponentID;
		size_t EntityID;
		size_t HashValue;
		bool enabled;
	public:

		IComponent() : enabled(true) {}
		virtual ~IComponent() {}

		inline const size_t GetComponentID() { return this->ComponentID; }
		inline const size_t GetOwner() { return this->EntityID; }
		inline void SetActive(bool state) { this->enabled = state; }
		inline bool IsActive() { return this->enabled; }
	};
}