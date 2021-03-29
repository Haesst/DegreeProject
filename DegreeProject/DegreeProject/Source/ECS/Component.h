#pragma once

#include "Types.h"

struct Component
{
	EntityID m_EntityID;
	Component() : m_EntityID() {}
	virtual ~Component() {}
	inline const EntityID getID() const { return m_EntityID; }
};