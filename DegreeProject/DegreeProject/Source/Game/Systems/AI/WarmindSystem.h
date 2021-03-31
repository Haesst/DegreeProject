#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Game/Components/Warmind.h"
#include "WarmindConsiderations.h"
#include "Game/Components/CharacterComponent.h"
#include "Game/Pathfinding.h"
#include "Game/Systems/UnitSystem.h"

struct WarmindSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	WarmindComponent* m_Warminds = nullptr;
	CharacterComponent* m_Characters = nullptr;
	Transform* m_Transforms = nullptr;
	UnitComponent* m_Units = nullptr;
	WarManager* m_WarManager = nullptr;

	WarmindSystem()
	{
		addComponentSignature<CharacterComponent>();
		m_EntityManager = &EntityManager::get();

		m_Units = m_EntityManager->getComponentArray<UnitComponent>();
		m_Warminds = nullptr;//m_EntityManager->getComponentArray<WarmindComponent>();
		m_Characters = m_EntityManager->getComponentArray<CharacterComponent>();
		m_Transforms = m_EntityManager->getComponentArray<Transform>();
		m_WarManager = &WarManager::get();
	}

	virtual void update() override
	{
		for (auto entity : m_Entities)
		{
			if (!m_Warminds[entity].m_Active)
			{
				continue;
			}

			if (m_Units[m_Characters[entity].m_UnitEntity].m_Raised)
			{
				m_Warminds[entity].m_OrderAccu += Time::deltaTime();

				if (m_Warminds[entity].m_OrderAccu >= m_Warminds[entity].m_OrderTimer)
				{
					m_Warminds[entity].m_OrderAccu = 0.0f;
					//considerOrders(entity, m_Units[m_Characters[entity].m_UnitEntity], m_Warminds[entity].m_Opponent);
				}
			}
		}
	}

	void moveUnit(EntityID unitToMove, Vector2DInt targetPosition)
	{
		UnitComponent& unit = m_Units[unitToMove];
		Transform& transform = m_EntityManager->getComponent<Transform>(unitToMove);

		unit.m_Moving = false;
		transform.m_Position = unit.m_LastPosition;

		Vector2D unitPosition = transform.m_Position;
		Vector2DInt startingPosition = Map::convertToMap(unitPosition);
		std::vector<Vector2DInt> path = Pathfinding::get().findPath(startingPosition, targetPosition);

		if (path.size() > 0)
		{
			if (unit.m_CurrentPath.size() > 0)
			{
				if (path.back() == unit.m_CurrentPath.back())
				{
					unit.m_Moving = true;
					return;
				}
			}

			unit.setPath(path, Map::convertToScreen(startingPosition));
		}
		else
		{
			unit.m_CurrentPath.clear();
		}
	}
};
