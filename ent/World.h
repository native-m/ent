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

	class World;

	template<class C, class... Components>
	class EntityQueryView
	{
	public:
		using EachFn = std::function<void(Entity&, C&, Components&...)>;

		class Iterator
		{
		public:
			Iterator(World* world, const std::bitset<MAX_COMPONENT>& componentMask, EntityId id, EntityId end) :
				m_world(world),
				m_componentMask(componentMask),
				m_id(id),
				m_end(end)
			{
				Next();
			}

			Iterator(const Iterator& i) :
				m_world(i.m_world),
				m_id(i.m_id),
				m_end(i.m_end)
			{
				Next();
			}

			Iterator& operator++()
			{
				++m_id;
				Next();
				return *this;
			}

			Iterator operator++(int)
			{
				Iterator tmp(*this);
				++(*this);
				return tmp;
			}

			bool operator==(const Iterator& other)
			{
				return m_id == other.m_id;
			}

			bool operator!=(const Iterator& other)
			{
				return m_id != other.m_id;
			}

			Entity operator*() const
			{
				return Entity(m_id, m_world);
			}

		private:
			World* m_world;
			std::bitset<MAX_COMPONENT> m_componentMask;
			EntityId m_id;
			EntityId m_end;

			void Next();
			bool IsValid();
		};

		EntityQueryView(World* world, const std::bitset<MAX_COMPONENT>& componentMask, EntityId idBegin, EntityId idEnd) :
			m_world(world),
			m_componentMask(componentMask),
			m_idBegin(idBegin),
			m_idEnd(idEnd)
		{
		}

		~EntityQueryView()
		{
		}

		Iterator begin()
		{
			return Iterator(m_world, m_componentMask, m_idBegin, m_idEnd);
		}

		Iterator end()
		{
			return Iterator(m_world, m_componentMask, m_idEnd, m_idEnd);
		}

		void Each(EachFn&& function);

	private:
		World* m_world;
		EntityId m_idBegin;
		EntityId m_idEnd;
		std::bitset<MAX_COMPONENT> m_componentMask;
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

		template<class C, class... Components>
		bool HasComponents(const Entity& e);

		template<class C>
		void DestroyComponent(const Entity& e);

		template<class C>
		C& GetComponent(const Entity& e);

		template<class C>
		const C& GetComponent(const Entity& e) const;

		std::bitset<MAX_COMPONENT>& GetComponentBits(const Entity& e);

		bool IsEntityValid(const Entity& e);

		template<class C, class... Components>
		EntityQueryView<C, Components...> QueryWithComponents();

		template<class C, class... Components>
		EntityQueryView<C, Components...> QueryWithComponents(unsigned int numEntities);

		virtual void Update(double dt);
		virtual void RenderScene(double dt);

		template<class C, class... Components>
		friend class EntityQueryView;

	private:
		std::deque<Entity> m_lastDestroyedEntities; // component garbage collector
		EntityId m_entityCounter;
		std::vector<EntityFlags> m_entityFlags;
		ComponentManager m_componentManager;
	};

	template<class C, class... Args>
	inline void World::AddComponent(const Entity& e, Args&&... args)
	{
		unsigned int componentId = Component<C>::id;
		std::bitset<MAX_COMPONENT>& componentBits = m_entityFlags[e.GetID()].componentBits;

		if (componentBits[componentId])
		{
			return;
		}

		m_componentManager.AddComponent<C>(e, std::forward<Args>(args)...);
		componentBits.set(componentId);
	}

	template<class C, class... Components>
	inline bool World::HasComponents(const Entity& e)
	{
		assert(m_entityFlags[e.GetID()].used && "Entity is not handled!");
		std::bitset<MAX_COMPONENT> componentMask;

		// build the component mask
		componentMask[Component<C>::id] = true;
		((componentMask[Component<Components>::id] = true), ...);

		return (m_entityFlags[e.GetID()].componentBits & componentMask) == componentMask;
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

	template<class C, class ...Components>
	inline EntityQueryView<C, Components...> World::QueryWithComponents()
	{
		std::bitset<MAX_COMPONENT> componentMask;

		// build the component mask
		componentMask[Component<C>::id] = true;
		((componentMask[Component<Components>::id] = true), ...);

		return EntityQueryView<C, Components...>(this, componentMask, 0, m_entityCounter);
	}

	template<class C, class ...Components>
	inline EntityQueryView<C, Components...> World::QueryWithComponents(unsigned int numEntities)
	{
		std::bitset<MAX_COMPONENT> componentMask;

		// build the component mask
		componentMask[Component<C>::id] = true;
		((componentMask[Component<Components>::id] = true), ...);

		return EntityQueryView<C, Components...>(this, componentMask, 0, (numEntities < m_entityCounter) ? numEntities : m_entityCounter);
	}

	template<class C, class ...Components>
	inline void EntityQueryView<C, Components...>::Iterator::Next()
	{
		while (m_id < m_end && !IsValid())
		{
			++m_id;
		}
	}

	template<class C, class ...Components>
	inline bool EntityQueryView<C, Components...>::Iterator::IsValid()
	{
		EntityFlags& f = m_world->m_entityFlags[m_id];
		return f.used && ((f.componentBits & m_componentMask) == m_componentMask);
	}

	template<class C, class ...Components>
	inline void EntityQueryView<C, Components...>::Each(EachFn&& function)
	{
		EntityQueryView& query = *this;
		for (auto entity : query)
		{
			function(entity, m_world->GetComponent<C>(entity), m_world->GetComponent<Components>(entity)...);
		}
	}
}