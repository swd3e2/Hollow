#pragma once
#include "../Platform.h"

namespace ECS { namespace util { namespace Internal {

	template<class T>
	class FamilyTypeID
	{
	private:
		static TypeID s_count;
	public:
		template<class U>
		static const TypeID Get()
		{
			return s_count++;
		}

		static const TypeID Get()
		{
			return s_count;
		}
	};

}}} // namespace ECS::util::Internal
