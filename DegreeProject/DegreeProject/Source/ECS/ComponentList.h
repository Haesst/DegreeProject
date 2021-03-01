#pragma once

#include <algorithm>

#include "Types.h"

class IComponentList
{
public:
	IComponentList() = default;
	~IComponentList() = default;
	virtual void Erase(const EntityID entity) {}
};

template<typename T>
class ComponentList : public IComponentList
{
public:
	std::vector<T> m_Components;

public:
	ComponentList() = default;
	~ComponentList() = default;

	void Insert(const T& component)
	{
		for (auto c : m_Components)
		{
			if (c.GetID() == component.GetID())
			{
				return;
			}
		}

		m_Components.push_back(component);
	}

	T& Get(const EntityID entity)
	{
		auto comp = std::find_if(m_Components.begin(), m_Components.end(), [&](const T& c{ return c.GetID() == entity; }));
		assert(comp != m_Data.end() && "Trying to get non-existing component!");
		return *comp;
	}

	void Erase(const EntityID entity) override final
	{
		auto comp = std::find_if(m_Components.begin(), m_Components.end(), [&](const T& c{ return c.GetID() == entity; }));

		if (comp != m_Components.end())
		{
			m_Components.erase(comp);
		}
	}
};