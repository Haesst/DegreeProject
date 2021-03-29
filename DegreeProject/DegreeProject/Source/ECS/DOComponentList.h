#pragma once

#include "Types.h"

class IComponentList
{
public:
	IComponentList() = default;
	~IComponentList() = default;
	virtual void erase(const EntityID) {}
};

template<typename T>
class DOComponentList : public IComponentList
{
public:
	T* m_Components;

public:
	DOComponentList()
		: IComponentList()
	{
		m_Components = new T[MAX_ENTITY_COUNT];
	};

	~DOComponentList() = default;

	void insert(const T& component, EntityID entity)
	{
		m_Components[entity] = component;
	}

	T& get(const EntityID entity)
	{
		return m_Components[entity];
	}

	void erase(const EntityID) override final
	{
		// Not necessary?
	}
};