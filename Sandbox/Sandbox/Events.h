#pragma once

#include <Hollow/Events/Event.h>

class ChangeSelectedEntity : public Hollow::Event<ChangeSelectedEntity>
{
public:
	int pickedId;
public:
	ChangeSelectedEntity(int pickedId) :
		pickedId(pickedId)
	{}
};