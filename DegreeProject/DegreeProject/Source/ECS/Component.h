#pragma once

#include "Types.h"

struct Component
{
protected:
	friend class EntityManager;
	EntityID m_EntityID;

public:
	Component() : m_EntityID() {}
	virtual ~Component() {}
	inline const EntityID GetID() const { return m_EntityID; }
};