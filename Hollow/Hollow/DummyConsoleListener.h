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
		EventSystem::instance()->addEventListener(new EventDelegate<DummyConsoleListener>(this, &DummyConsoleListener::onWindowCreate, WindowCreateEvent::getStaticEventId()));
		EventSystem::instance()->addEventListener(new EventDelegate<DummyConsoleListener>(this, &DummyConsoleListener::onButtonReleased, ButtonReleasedEvent::getStaticEventId()));
		EventSystem::instance()->addEventListener(new EventDelegate<DummyConsoleListener>(this, &DummyConsoleListener::onButtonPressed, ButtonPressedEvent::getStaticEventId()));
	}

	void onWindowCreate(IEvent* ev)
	{
		WindowCreateEvent* windowEvent = (WindowCreateEvent*)ev;
		HW_INFO("Window created {}", windowEvent->message);
	}

	void onButtonPressed(IEvent* ev)
	{
		ButtonPressedEvent* windowEvent = (ButtonPressedEvent*)ev;
		HW_INFO("Button pressed {}", windowEvent->button);
	}

	void onButtonReleased(IEvent* ev)
	{
		ButtonReleasedEvent* windowEvent = (ButtonReleasedEvent*)ev;
		HW_INFO("Button pressed {}", windowEvent->button);
	}
};