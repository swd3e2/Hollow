#pragma once
#include "Hollow/ECS/Components/Component.h"

class SelectComponent : public Hollow::Component<SelectComponent>
{
public:
	SelectComponent(bool selected) : selected(selected) {}
	bool selected;
};
