#pragma once
#include "Events/IEventListener.h"
#include "Events/EventSystem.h"
#include "Events/ButtonPressedEvent.h"
#include "Events/ButtonReleasedEvent.h"
#include "Events/WindowCreateEvent.h"
#include "Common/Log.h"

class DummyConsoleListener : public IEventListener
{
private:
public:
	DummyConsoleListener()
	{
	}
};