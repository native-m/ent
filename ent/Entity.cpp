#include "Entity.h"
#include "World.h"

namespace Hx
{
	std::bitset<MAX_COMPONENT>& Entity::GetComponentBits() const
	{
		return m_world->GetComponentBits(*this);
	}
}
