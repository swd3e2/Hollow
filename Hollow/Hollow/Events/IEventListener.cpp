#include "IEventListener.h"

namespace Hollow {
	IEventListener::IEventListener()
	{}

	IEventListener::~IEventListener()
	{
		this->UnregisterAllEventsCallback();
	}

	void IEventListener::UnregisterAllEventsCallback()
	{
		for (auto it : this->m_RegisteredCallbacks)
		{
			EventHandler::Get()->RemoveEventCallback(it);
		}
		this->m_RegisteredCallbacks.clear();
	}
}
