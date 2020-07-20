#pragma once

#include "Def.h"
#include <bitset>

namespace Hx
{
	using EntityId = unsigned long long;

	class World;

	class Entity
	{
	public:
		Entity() :
			m_id(0),
			m_world(nullptr)
		{
		}

		Entity(EntityId id, World* world) :
			m_id(id),
			m_world(world)
		{
		}

		Entity(const Entity& e) :
			m_id(e.m_id),
			m_world(e.m_world)
		{
		}

		Entity& operator=(const Entity& e)
		{
			m_id = e.m_id;
			m_world = e.m_world;
			return *this;
		}

		EntityId GetID() const
		{
			return m_id;
		}

		constexpr bool operator()(const Entity& a, const Entity& b) const
		{
			return (a.m_id < b.m_id);
		}

		template<class C, class... Args>
		Entity& AddComponent(Args&&... args)
		{
			m_world->AddComponent<C>(*this, std::forward<Args>(args)...);
			return *this;
		}

		template<class C, class... Components>
		bool HasComponents() const
		{
			return m_world->HasComponents<C, Components...>(*this);
		}

		template<class C>
		C& GetComponent()
		{
			return m_world->GetComponent<C>(*this);
		}

		template<class C>
		const C& GetComponent() const
		{
			return m_world->GetComponent<C>(*this);
		}

		std::bitset<MAX_COMPONENT>& GetComponentBits() const;

	private:
		EntityId m_id;
		World* m_world;
	};
}