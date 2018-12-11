#pragma once

#ifndef __GAME_OBJECT__
#define __GAME_OBJECT__

#include "Entity.h"

namespace Hollow {

	class GameObject : public Entity<GameObject>
	{
	};

}

#endif