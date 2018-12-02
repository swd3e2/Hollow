#include "IComponent.h"

namespace ECS {

	IComponent::IComponent() :
		m_Owner(INVALID_ENTITY_ID),
		m_Enabled(true)
	{}

	IComponent::~IComponent()
	{}

}