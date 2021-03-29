#pragma once

#include <algorithm>

#include "Types.h"

class IComponentList
{
public:
	IComponentList() = default;
	~IComponentList() = default;
	virtual void erase(const EntityID entity) {}
};

template<typename T>
class ComponentList : public IComponentList
{
public:
	std::vector<T> m_Components;

public:
	ComponentList() = default;
	~ComponentList() = default;

	void insert(const T& component)
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

	T& get(const EntityID entity)
	{
		auto comp = std::find_if(m_Components.begin(), m_Components.end(), [&](const T& c) { return c.GetID() == entity; });
		assert(comp != m_Components.end() && "Trying to get non-existing component!");
		return *comp;
	}

	void erase(const EntityID entity) override final
	{
		auto comp = std::find_if(m_Components.begin(), m_Components.end(), [&](const T& c) { return c.GetID() == entity; });

		if (comp != m_Components.end())
		{
			m_Components.erase(comp);
		}
	}
};