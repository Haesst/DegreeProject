#pragma once

#include "Types.h"

class System
{
public:
	EntitySignature m_Signature;
	std::set<EntityID> m_Entities;
	System() = default;
	virtual ~System() = default;

	void removeEntity(const EntityID entity)
	{
		m_Entities.erase(entity);
	}

	void addEntity(const EntityID entity)
	{
		m_Entities.insert(entity);
	}

	const EntitySignature getSignature() const
	{
		return m_Signature;
	}

	template<typename T>
	void addComponentSignature()
	{
		m_Signature.insert(componentType<T>());
	}

	const std::set<EntityID> getEntities()
	{
		return m_Entities;
	}

	virtual void start() {}
	virtual void update() {}
	virtual void render() {}
	virtual void destroy() {}
};