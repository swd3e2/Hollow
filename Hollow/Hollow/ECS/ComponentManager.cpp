#include "ComponentManager.h"

namespace Hollow {

	ComponentManager* ComponentManager::_instance = nullptr;

	ComponentManager::ComponentManager()
	{
		if (_instance == nullptr) {
			_instance = this;
		}

		HW_DEBUG("ComponentManager: created");
		const size_t NUM_COMPONENTS{ Core::Utils::FamilyTypeID<IComponent>::Get() };

		this->m_EntityComponentMap.resize(ENITY_LUT_GROW);
		for (auto i = 0; i < ENITY_LUT_GROW; ++i)
			this->m_EntityComponentMap[i].resize(NUM_COMPONENTS, INVALID_OBJECT_ID);
	}

	ComponentManager::~ComponentManager()
	{
		for (auto cc : this->m_ComponentContainerRegistry)
		{
			delete cc.second;
			cc.second = nullptr;
		}
	}

	ComponentId ComponentManager::AqcuireComponentId(IComponent* component)
	{
		int i = 0;
		for (; i < this->m_ComponentLUT.size(); ++i)
		{
			if (this->m_ComponentLUT[i] == nullptr)
			{
				this->m_ComponentLUT[i] = component;
				return i;
			}
		}

		// increase component LUT size
		this->m_ComponentLUT.resize(this->m_ComponentLUT.size() + COMPONENT_LUT_GROW, nullptr);

		this->m_ComponentLUT[i] = component;
		return i;
	}

	void ComponentManager::ReleaseComponentId(ComponentId id)
	{
		assert((id != INVALID_OBJECT_ID && id < this->m_ComponentLUT.size()) && "Invalid component id");
		this->m_ComponentLUT[id] = nullptr;
	}

	void ComponentManager::MapEntityComponent(EntityID entityId, ComponentId componentId, ComponentTypeID componentTypeId)
	{
		static const size_t NUM_COMPONENTS{ Core::Utils::FamilyTypeID<IComponent>::Get() };

		if ((this->m_EntityComponentMap.size() - 1) < entityId)
		{
			size_t oldSize = this->m_EntityComponentMap.size();

			// we scale this map size along the entity lookup table size
			size_t newSize = oldSize + ENITY_LUT_GROW;

			this->m_EntityComponentMap.resize(newSize);

			for (auto i = oldSize; i < newSize; ++i)
				this->m_EntityComponentMap[i].resize(NUM_COMPONENTS, INVALID_OBJECT_ID);
		}

		// create mapping
		this->m_EntityComponentMap[entityId][componentTypeId] = componentId;
	}

	void ComponentManager::UnmapEntityComponent(EntityID entityId, ComponentId componentId, ComponentTypeID componentTypeId)
	{
		assert(this->m_EntityComponentMap[entityId][componentTypeId] == componentId && "FATAL: Entity Component ID mapping corruption!");

		// free mapping
		this->m_EntityComponentMap[entityId][componentTypeId] = INVALID_OBJECT_ID;

		// free component id
		this->ReleaseComponentId(componentId);
	}

} // namespace ECS