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
	void addComponentList()
	{
		const ComponentTypeID type = componentType<T>();

		assert(m_ComponentArrays.find(type) == m_ComponentArrays.end() && "Component list already registered!");
		m_ComponentArrays[type] = std::move(std::make_shared<DOComponentList<T>>());
	}

	void addEntitySignature(const EntityID entity)
	{
		assert(m_EntitySignatures.find(entity) == m_EntitySignatures.end() && "Entity already has registered signature");
		m_EntitySignatures[entity] = std::move(std::make_shared<EntitySignature>());
	}

	std::shared_ptr<EntitySignature> getEntitySignature(const EntityID entity)
	{
		assert(m_EntitySignatures.find(entity) != m_EntitySignatures.end() && "Signature for entity not found");
		return m_EntitySignatures.at(entity);
	}

	void updateEntityTargetSystems(const EntityID entity)
	{
		for (auto& system : m_RegisteredSystems)
		{
			addEntityToSystem(entity, system.second.get());
		}
	}

	void addEntityToSystem(const EntityID entity, System* system)
	{
		if (belongToSystem(entity, system->m_Signature) && isEntityActive(entity))
		{
			system->m_Entities.insert(entity);
		}
		else
		{
			system->m_Entities.erase(entity);
		}
	}

	bool belongToSystem(const EntityID entity, const EntitySignature& systemSignature)
	{
		for (const auto componentType : systemSignature)
		{
			if (getEntitySignature(entity)->count(componentType) == (size_t)0)
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

	inline static EntityManager& get()
	{
		if (m_Instance == nullptr)
		{
			m_Instance = new EntityManager();
		}

		return *m_Instance;
	}

	void start()
	{
		for (auto& system : m_RegisteredSystems)
		{
			system.second->start();
		}
	}

	void update()
	{
		for (auto& system : m_RegisteredSystems)
		{
			system.second->update();
		}
	}

	void render()
	{
		for (auto& system : m_RegisteredSystems)
		{
 			system.second->render();
		}
	}

	const EntityID addNewEntity()
	{
		const EntityID entityID = m_AvailableEntityIDs.front();
		addEntitySignature(entityID);

		addComponent<Transform>(entityID);

		m_AvailableEntityIDs.pop();
		m_EntityCount++;

		m_ActiveEntities[entityID] = true;
		m_EntityTags[entityID] = EntityTag::Untagged;

		return entityID;
	}

	void destroyEntity(const EntityID entity)
	{
		assertEntityNotOutOfRange(entity);

		m_EntitySignatures.erase(entity);

		for (auto& array : m_ComponentArrays)
		{
			array.second->erase(entity);
		}

		for (auto& system : m_RegisteredSystems)
		{
			system.second->removeEntity(entity);
		}

		m_ActiveEntities.erase(entity);
		m_EntityTags.erase(entity);

		m_EntityCount--;
		m_AvailableEntityIDs.push(entity);
	}

	void setEntityActive(const EntityID entity, const bool active)
	{
		m_ActiveEntities[entity] = active;
		updateEntityTargetSystems(entity);
	}

	bool isEntityActive(const EntityID entity)
	{
		return m_ActiveEntities[entity];
	}

	void setEntityTag(const EntityID entity, const EntityTag tag)
	{
		m_EntityTags[entity] = tag;
	}

	EntityTag getEntityTag(const EntityID entity)
	{
		return m_EntityTags[entity];
	}

	template<typename T, typename... Args>
	void addComponent(const EntityID entity, Args&&... args)
	{
		assertEntityNotOutOfRange(entity);
		assert(m_EntitySignatures[entity]->size() < MAX_COMPONENT_COUNT && "Component count limit reached!");

		T component(std::forward<Args>(args)...);
		component.m_EntityID = entity;
		getEntitySignature(entity)->insert(componentType<T>());
		getComponentList<T>()->insert(component, entity);

		const ComponentTypeID type = componentType<T>();
		m_EntitySignatures.at(entity)->insert(type);
		updateEntityTargetSystems(entity);
	}

	template<typename T>
	void removeComponent(const EntityID entity)
	{
		assertEntityNotOutOfRange(entity);

		const ComponentTypeID componentType = componentType<T>();
		m_EntitySignatures.at(entity)->erase(componentType);
		getComponentList<T>()->Erase(entity);
		updateEntityTargetSystems(entity);
	}

	template<typename T>
	T& getComponent(const EntityID entity)
	{
		assertEntityNotOutOfRange(entity);

		const ComponentTypeID type = componentType<T>();
		return getComponentList<T>()->get(entity);
	}

	template<typename T>
	T* getComponentArray()
	{
		const ComponentTypeID type = componentType<T>();
		// return nullptr;
		return getComponentList<T>()->m_Components;
	}

	template<typename T>
	std::shared_ptr<DOComponentList<T>> getComponentList()
	{
		const ComponentTypeID type = componentType<T>();

		if (m_ComponentArrays.count(type) == (size_t)0)
		{
			addComponentList<T>();
		}

		return std::static_pointer_cast<DOComponentList<T>>(m_ComponentArrays.at(type));
	}

	template<typename T>
	const bool hasComponent(const EntityID entity)
	{
		assertEntityNotOutOfRange(entity);

		return (getEntitySignature(entity)->count(componentType<T>()) > 0);
	}

	void assertEntityNotOutOfRange(const EntityID entity)
	{
		assert(entity < MAX_ENTITY_COUNT && "EntityID out of range");
	}

	template<typename T>
	void registerSystem()
	{
		const SystemTypeID type = systemType<T>();
		assert(m_RegisteredSystems.count(type) == (size_t)0 && "System already registered");
		auto system = std::make_shared<T>();

		// Add entity to new system
		for (EntityID entity = 0; entity < m_EntityCount; entity++)
		{
			addEntityToSystem(entity, system.get());
		}

		m_RegisteredSystems[type] = std::move(system);
	}

	template<typename T>
	std::shared_ptr<System> getSystem()
	{
		const SystemTypeID type = systemType<T>();
		assert(m_RegisteredSystems.count(type) != 0 && "System not registered!");
		return m_RegisteredSystems[type];
	}

	template<typename T>
	void unregisterSystem()
	{
		const SystemTypeID type = systemType<T>();
		assert(m_RegisteredSystems.count(type) != 0 && "System not registered!");
		m_RegisteredSystems.erase(type);
	}
};