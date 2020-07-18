#pragma once

#include <cassert>
#include <iostream>
#include <map>
#include <queue>
#include <bitset>
#include "Def.h"
#include "Entity.h"
#include "Component.h"
#include "ComponentManager.h"

namespace Hx
{
	struct EntityFlags
	{
		std::bitset<MAX_COMPONENT> componentBits;
		bool used;
	};

	class World
	{
	public:
		World(size_t numEntity);
		virtual ~World();

		Entity CreateEntity();
		void DestroyEntity(const Entity& e);
		void DestroyAllEntity();

		template<class C, class... Args>
		void AddComponent(const Entity& e, Args&&... args);

		template<class C>
		bool HasComponent(const Entity& e);

		template<class C>
		void DestroyComponent(const Entity& e);

		template<class C>
		C& GetComponent(const Entity& e);

		template<class C>
		const C& GetComponent(const Entity& e) const;

		std::bitset<MAX_COMPONENT>& GetComponentBits(const Entity& e);

		virtual void Update(double dt);
		virtual void RenderScene(double dt);

	private:
		std::deque<Entity> m_lastDestroyedEntities; // component garbage collector
		EntityId m_entityCounter;
		std::vector<EntityFlags> m_entityFlags;
		ComponentManager m_componentManager;
	};

	template<class C, class ...Args>
	inline void World::AddComponent(const Entity& e, Args&& ...args)
	{
		unsigned int componentId = Component<C>::id;
		std::bitset<MAX_COMPONENT>& componentBits = m_entityFlags[e.GetID()].componentBits;

		if (componentBits.test(componentId))
		{
			return;
		}

		m_componentManager.AddComponent<C>(e, std::forward<Args>(args)...);
		componentBits.set(componentId);
	}

	template<class C>
	inline bool World::HasComponent(const Entity& e)
	{
		assert(m_entityFlags[e.GetID()].used && "Entity is not handled!");
		unsigned int componentId = Component<C>::id;
		return m_entityFlags[e.GetID()].componentBits.test(componentId);
	}

	template<class C>
	inline void World::DestroyComponent(const Entity& e)
	{
		assert(m_entityFlags[e.GetID()].used && "Entity is not handled!");
		m_componentManager.DestroyComponent<C>(e);
	}

	template<class C>
	inline C& World::GetComponent(const Entity& e)
	{
		assert(m_entityFlags[e.GetID()].used && "Entity is not handled!");
		return m_componentManager.GetComponent<C>(e);
	}

	template<class C>
	inline const C& World::GetComponent(const Entity& e) const
	{
		assert(m_entityFlags[e.GetID()].used && "Entity is not handled!");
		return m_componentManager.GetComponent<C>(e);
	}
}