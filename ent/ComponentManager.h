#pragma once

#include <vector>
#include <memory>
#include <functional>
#include "Entity.h"
#include "Component.h"
#include "ComponentPool.h"

namespace Hx
{
	using DestructorInvoker = std::function<void(void*)>;

	class ComponentManager
	{
	public:
		ComponentManager(size_t numEntity);
		~ComponentManager();

		template<class C>
		void RegisterComponentPool();

		template<class C, class... Args>
		void AddComponent(const Entity& e, Args&& ...args);

		template<class C>
		C& GetComponent(const Entity& e);

		template<class C>
		void DestroyComponent(const Entity& e);

		void DestroyAllComponents(const Entity& e);

	private:
		size_t m_numEntity;
		std::vector<std::unique_ptr<ComponentPoolBase>> m_componentPools;
	};

	template<class C>
	inline void ComponentManager::RegisterComponentPool()
	{
		unsigned int componentId = Component<C>::id;

		if (componentId < m_componentPools.size())
		{
			return;
		}

		m_componentPools.push_back(std::unique_ptr<ComponentPoolBase>(new ComponentPool<C>));
	}

	template<class C, class... Args>
	inline void ComponentManager::AddComponent(const Entity& e, Args&& ...args)
	{
		unsigned int componentId = Component<C>::id;
		ComponentPoolBase* pool = nullptr;
		C* component = nullptr;
		EntityId id = e.GetID();

		// Register the component pool if it's not already registered
		RegisterComponentPool<C>();

		// Get the component pool based on the component ID
		pool = m_componentPools[componentId].get();

		// Assign the component to the pool
		pool->Resize(id + 1); // Resize if needed
		component = reinterpret_cast<C*>(pool->Get(id));
		new(component) C(std::forward<Args>(args)...);
	}

	template<class C>
	inline C& ComponentManager::GetComponent(const Entity& e)
	{
		unsigned int componentId = Component<C>::id;
		ComponentPoolBase* pool = m_componentPools[componentId].get();
		C* component = nullptr;

		component = reinterpret_cast<C*>(pool->Get(e.GetID()));
		return *component;
	}

	template<class C>
	inline void ComponentManager::DestroyComponent(const Entity& e)
	{
		unsigned int componentId = Component<C>::id;
		ComponentPoolBase* pool = m_componentPools[componentId].get();

		pool->Destroy(e.GetID());
	}
}
