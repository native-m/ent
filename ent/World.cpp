#include "World.h"
#include <random>
#include <cassert>

namespace Hx
{
	World::World(size_t numEntity) :
		m_entityCounter(0),
		m_componentManager(numEntity)
	{
		m_entityFlags.resize(numEntity);
	}

	World::~World()
	{
		DestroyAllEntity();
	}

	Entity World::CreateEntity()
	{
		Entity ent(m_entityCounter, this);

		// use last destroyed entities
		if (!m_lastDestroyedEntities.empty())
		{
			Entity tmp(m_lastDestroyedEntities.front());
			m_lastDestroyedEntities.pop_front();
			m_entityFlags[tmp.GetID()].used = true;
			return tmp;
		}

		m_entityFlags[m_entityCounter].used = true;
		m_entityCounter++;

		return ent;
	}

	void World::DestroyEntity(const Entity& e)
	{
		if (!m_entityFlags[e.GetID()].used)
		{
			return; // the entity is already destroyed
		}

		m_componentManager.DestroyAllComponents(e);
		m_entityFlags[e.GetID()].componentBits.reset();
		m_entityFlags[e.GetID()].used = false; // mark unused
		m_lastDestroyedEntities.push_back(e);
	}

	void World::DestroyAllEntity()
	{
		for (EntityId i = 0; i < m_entityCounter; i++)
		{
			if (!m_entityFlags[i].used)
			{
				continue;
			}

			m_componentManager.DestroyAllComponents(Entity(i, this));
			m_entityFlags[i].componentBits.reset();
			m_entityFlags[i].used = false;
		}
	}
	
	std::bitset<MAX_COMPONENT>& World::GetComponentBits(const Entity& e)
	{
		return m_entityFlags[e.GetID()].componentBits;
	}

	bool World::IsEntityValid(const Entity& e)
	{
		return m_entityFlags[e.GetID()].used;
	}

	void World::Update(double dt)
	{
	}

	void World::RenderScene(double dt)
	{
	}
}

