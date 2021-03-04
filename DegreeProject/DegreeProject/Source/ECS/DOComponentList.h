#pragma once

#include "Types.h"

class IComponentList
{
public:
	IComponentList() = default;
	~IComponentList() = default;
	virtual void Erase(const EntityID entity) {}
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

	void Insert(const T& component, EntityID entity)
	{
		m_Components[entity] = component;
	}

	T& Get(const EntityID entity)
	{
		return m_Components[entity];
	}

	void Erase(const EntityID entity) override final
	{
		// Not necessary?
	}
};