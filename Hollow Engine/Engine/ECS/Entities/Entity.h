#pragma once

#ifndef __ENTITY__
#define __ENTITY__

namespace Hollow { namespace Core {

	template <class E>
	class Entity
	{
	public:
		Entity(unsigned int id) :
			EntityID(id)
		{}

		const unsigned int EntityID;
	};

}}

#endif