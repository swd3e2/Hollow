#ifndef __MOVE_COMPONENT__
#define __MOVE_COMPONENT__
#pragma once
#include "Component.h"

namespace Hollow {

	class MoveComponent : public Component<MoveComponent>
	{
	public:
		MoveComponent() : move(true) {}
		bool move;
	};

}

#endif