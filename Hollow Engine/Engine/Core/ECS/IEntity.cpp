#include "IEntity.h"

namespace ECS
{
	IEntity::IEntity() :
		m_Active(true)
	{}

	IEntity::~IEntity()
	{}

	void IEntity::SetActive(bool active)
	{
		if (this->m_Active == active)
			return;

		if (active == false)
		{
			this->OnDisable();
		}
		else
		{
			this->OnEnable();
		}

		this->m_Active = active;
	}
}