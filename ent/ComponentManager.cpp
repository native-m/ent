#include "ComponentManager.h"

namespace Hx
{
	ComponentManager::ComponentManager(size_t numEntity) :
		m_numEntity(numEntity)
	{
	}

	ComponentManager::~ComponentManager()
	{
	}

	void ComponentManager::DestroyAllComponents(const Entity& e)
	{
		EntityId id = e.GetID();
		unsigned int componentId = 0;

		for (auto& componentPool : m_componentPools)
		{
			std::bitset<MAX_COMPONENT>& bits = e.GetComponentBits();

			if (bits.test(componentId))
			{
				ComponentPoolBase* pool = componentPool.get();
				pool->Destroy(id);
			}

			componentId++;
		}
	}
}
