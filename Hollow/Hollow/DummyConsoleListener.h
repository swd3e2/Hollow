#pragma once
#include "Events/IEventListener.h"
#include "Events/EventSystem.h"
#include "Events/ButtonPressedEvent.h"
#include "Events/ButtonReleasedEvent.h"
#include "Events/WindowCreateEvent.h"
#include "Graphics/Events/BeginFrameEvent.h"
#include "Graphics/Events/EndFrameEvent.h"
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
		EventSystem::instance()->addEventListener(new EventDelegate<DummyConsoleListener>(this, &DummyConsoleListener::onButtonPressed, BeginFrameEvent::getStaticEventId()));
		EventSystem::instance()->addEventListener(new EventDelegate<DummyConsoleListener>(this, &DummyConsoleListener::onButtonPressed, EndFrameEvent::getStaticEventId()));
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

	void onBeginFrame(IEvent* ev)
	{
		BeginFrameEvent* windowEvent = (BeginFrameEvent*)ev;
		HW_INFO("Begin frame");
	}

	void onEndFrame(IEvent* ev)
	{
		EndFrameEvent* windowEvent = (EndFrameEvent*)ev;
		HW_INFO("End frame");
	}
};