#pragma once
#include <iostream>
#include "Ids.h"
#include "Event.h"
#include "ButtonPressEvent.h"
#include "IEventListener.h"
#include "EventSystem.h"

class A : public Event<A>
{
public:
	std::string kek = "somestrinrg";
};

class B : public Event<B>
{
public:
	std::string kek = "somestrinrgb";
};

class ex : public IEventListener
{
public:
	void onmessage(IEvent* e) { std::cout << ((A*)e)->kek.c_str() << std::endl; }
	void onmessageb(IEvent* e) { std::cout << ((B*)e)->kek.c_str() << std::endl; }
};

int main()
{
	A a;
	B b;
	ex e;
	
	EventSystem eventSystem;
	eventSystem.addEventListener<ex>(&e, &ex::onmessage, a.getEventId());
	eventSystem.addEventListener(new EventDelegate<ex>(&e, &ex::onmessage, A::getStaticEventId()));

	eventSystem.addEvent(&a);
	eventSystem.addEvent(&a);
	eventSystem.addEvent(&a);

	eventSystem.dispatch();

	system("pause");
	return 0;
}

