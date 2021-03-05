#pragma once

#include <queue>
#include <map>
#include <memory>
#include <cassert>

#include "Types.h"
#include "System.h"
#include "Component.h"
#include "DOComponentList.h"
#include "EntityTag.h"
#include "Components/Transform.h"
#include "Engine/Window.h"

class EntityManager
{
private:
	EntityID m_EntityCount;
	std::queue<EntityID> m_AvailableEntityIDs;
	std::map<EntityID, std::shared_ptr<EntitySignature>> m_EntitySignatures;
	std::map<SystemTypeID, std::shared_ptr<System>> m_RegisteredSystems;
	std::map<SystemTypeID, std::shared_ptr<IComponentList>> m_ComponentArrays;
	std::map<EntityID, bool> m_ActiveEntities;
	std::map<EntityID, EntityTag> m_EntityTags;

	static EntityManager* m_Instance;

private:

	template<typename T>
	void AddComponentList()
	{
		const ComponentTypeID componentType = ComponentType<T>();

		assert(m_ComponentArrays.find(componentType) == m_ComponentArrays.end() && "Component list already registered!");
		m_ComponentArrays[componentType] = std::move(std::make_shared<DOComponentList<T>>());
	}

	void AddEntitySignature(const EntityID entity)
	{
		assert(m_EntitySignatures.find(entity) == m_EntitySignatures.end() && "Entity already has registered signature");
		m_EntitySignatures[entity] = std::move(std::make_shared<EntitySignature>());
	}

	std::shared_ptr<EntitySignature> GetEntitySignature(const EntityID entity)
	{
		assert(m_EntitySignatures.find(entity) != m_EntitySignatures.end() && "Signature for entity not found");
		return m_EntitySignatures.at(entity);
	}

	void UpdateEntityTargetSystems(const EntityID entity)
	{
		for (auto& system : m_RegisteredSystems)
		{
			AddEntityToSystem(entity, system.second.get());
		}
	}

	void AddEntityToSystem(const EntityID entity, System* system)
	{
		if (BelongToSystem(entity, system->m_Signature) && IsEntityActive(entity))
		{
			system->m_Entities.insert(entity);
		}
		else
		{
			system->m_Entities.erase(entity);
		}
	}

	bool BelongToSystem(const EntityID entity, const EntitySignature& systemSignature)
	{
		for (const auto componentType : systemSignature)
		{
			if (GetEntitySignature(entity)->count(componentType) == 0)
			{
				return false;
			}
		}

		return true;
	}

public:
	EntityManager()
		: m_EntityCount(0)
	{
		for (EntityID entity = 0u; entity < MAX_ENTITY_COUNT; entity++)
		{
			m_AvailableEntityIDs.push(entity);
		}
	}

	~EntityManager()
	{
		delete m_Instance;
	}

	inline static EntityManager& Get()
	{
		if (m_Instance == nullptr)
		{
			m_Instance = new EntityManager();
		}

		return *m_Instance;
	}

	void Update()
	{
		for (auto& system : m_RegisteredSystems)
		{
			system.second->Update();
		}
	}

	void Render()
	{
		Window::GetWindow()->clear(sf::Color::Blue);
		for (auto& system : m_RegisteredSystems)
		{
			system.second->Render();
		}
		Window::GetWindow()->display();
	}

	const EntityID AddNewEntity()
	{
		const EntityID entityID = m_AvailableEntityIDs.front();
		AddEntitySignature(entityID);

		AddComponent<Transform>(entityID);

		m_AvailableEntityIDs.pop();
		m_EntityCount++;

		m_ActiveEntities[entityID] = true;
		m_EntityTags[entityID] = EntityTag::Untagged;

		return entityID;
	}

	void DestroyEntity(const EntityID entity)
	{
		AssertEntityNotOutOfRange(entity);

		m_EntitySignatures.erase(entity);

		for (auto& array : m_ComponentArrays)
		{
			array.second->Erase(entity);
		}

		for (auto& system : m_RegisteredSystems)
		{
			system.second->RemoveEntity(entity);
		}

		m_ActiveEntities.erase(entity);
		m_EntityTags.erase(entity);

		m_EntityCount--;
		m_AvailableEntityIDs.push(entity);
	}

	void SetEntityActive(const EntityID entity, const bool active)
	{
		m_ActiveEntities[entity] = active;
		UpdateEntityTargetSystems(entity);
	}

	bool IsEntityActive(const EntityID entity)
	{
		return m_ActiveEntities[entity];
	}

	void SetEntityTag(const EntityID entity, const EntityTag tag)
	{
		m_EntityTags[entity] = tag;
	}

	EntityTag GetEntityTag(const EntityID entity)
	{
		return m_EntityTags[entity];
	}

	template<typename T, typename... Args>
	void AddComponent(const EntityID entity, Args&&... args)
	{
		AssertEntityNotOutOfRange(entity);
		assert(m_EntitySignatures[entity]->size() < MAX_COMPONENT_COUNT && "Component count limit reached!");

		T component(std::forward<Args>(args)...);
		component.m_EntityID = entity;
		GetEntitySignature(entity)->insert(ComponentType<T>());
		GetComponentList<T>()->Insert(component, entity);

		const ComponentTypeID componentType = ComponentType<T>();
		m_EntitySignatures.at(entity)->insert(componentType);
		UpdateEntityTargetSystems(entity);
	}

	template<typename T>
	void RemoveComponent(const EntityID entity)
	{
		AssertEntityNotOutOfRange(entity);

		const ComponentTypeID componentType = ComponentType<T>();
		m_EntitySignatures.at(entity)->erase(componentType);
		GetComponentList<T>()->Erase(entity);
		UpdateEntityTargetSystems(entity);
	}

	template<typename T>
	T& GetComponent(const EntityID entity)
	{
		AssertEntityNotOutOfRange(entity);

		const ComponentTypeID componentType = ComponentType<T>();
		return GetComponentList<T>()->Get(entity);
	}

	template<typename T>
	T* GetComponentArray()
	{
		const ComponentTypeID componentType = ComponentType<T>();
		// return nullptr;
		return GetComponentList<T>()->m_Components;
	}

	template<typename T>
	std::shared_ptr<DOComponentList<T>> GetComponentList()
	{
		const ComponentTypeID componentType = ComponentType<T>();

		if (m_ComponentArrays.count(componentType) == 0)
		{
			AddComponentList<T>();
		}

		return std::static_pointer_cast<DOComponentList<T>>(m_ComponentArrays.at(componentType));
	}

	template<typename T>
	const bool HasComponent(const EntityID entity)
	{
		AssertEntityNotOutOfRange(entity);

		return (GetEntitySignature(entity)->count(ComponentType<T>()) > 0);
	}

	void AssertEntityNotOutOfRange(const EntityID entity)
	{
		assert(entity < MAX_ENTITY_COUNT && "EntityID out of range");
	}

	template<typename T>
	void RegisterSystem()
	{
		const SystemTypeID systemType = SystemType<T>();
		assert(m_RegisteredSystems.count(systemType) == 0 && "System already registered");
		auto system = std::make_shared<T>();

		// Add entity to new system
		for (EntityID entity = 0; entity < m_EntityCount; entity++)
		{
			AddEntityToSystem(entity, system.get());
		}

		system->Start();
		m_RegisteredSystems[systemType] = std::move(system);
	}

	template<typename T>
	std::shared_ptr<System> GetSystem()
	{
		const SystemTypeID systemType = SystemType<T>();
		assert(m_RegisteredSystems.count(systemType) != 0 && "System not registered!");
		return m_RegisteredSystems[systemType];
	}

	template<typename T>
	void UnregisterSystem()
	{
		const SystemTypeID systemType = SystemType<T>();
		assert(m_RegisteredSystems.count(systemType) != 0 && "System not registered!");
		m_RegisteredSystems.erase(systemType);
	}
};