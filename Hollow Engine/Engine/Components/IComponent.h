#pragma once

#ifndef __I_COMPONENT__
#define __I_COMPONENT__

namespace Hollow {
	class ComponentManager;

	class IComponent
	{
		friend ComponentManager;
	protected:
		unsigned int ComponentID;
		unsigned int EntityID;
		unsigned int HashValue;
		bool enabled;
	public:

		IComponent() : enabled(true) {}
		virtual ~IComponent() {}

		inline const unsigned int GetComponentID() { return this->ComponentID; }
		inline const unsigned int GetOwner() { return this->EntityID; }
		inline void SetActive(bool state) { this->enabled = state; }
		inline bool IsActive() { return this->enabled; }
	};
}
#endif