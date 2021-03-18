#pragma once

#include "Types.h"

class System
{
public:
	EntitySignature m_Signature;
	std::set<EntityID> m_Entities;
	System() = default;
	virtual ~System() = default;

	void RemoveEntity(const EntityID entity)
	{
		m_Entities.erase(entity);
	}

	void AddEntity(const EntityID entity)
	{
		m_Entities.insert(entity);
	}

	const EntitySignature GetSignature() const
	{
		return m_Signature;
	}

	template<typename T>
	void AddComponentSignature()
	{
		m_Signature.insert(ComponentType<T>());
	}

	const std::set<EntityID> GetEntities()
	{
		return m_Entities;
	}

	virtual void Start() {}
	virtual void Update() {}
	virtual void Render() {}
	virtual void Destroy() {}
};