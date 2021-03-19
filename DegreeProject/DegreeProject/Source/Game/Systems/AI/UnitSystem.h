#pragma once
#include "ECS/System.h"
#include <Game\Components\Unit.h>
#include <Engine\AssetHandler.h>

struct UnitSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	UnitComponent* m_UnitComponents = nullptr;
	CharacterComponent* m_Characters = nullptr;
	WarmindComponent* m_Warminds = nullptr;

	float m_MoveTolerance = 0.3f;

	UnitSystem()
	{
		AddComponentSignature<UnitComponent>();
		m_EntityManager = &EntityManager::Get();
		m_UnitComponents = m_EntityManager->GetComponentArray<UnitComponent>();
		m_Warminds = m_EntityManager->GetComponentArray<WarmindComponent>();
		m_Characters = m_EntityManager->GetComponentArray<CharacterComponent>();
	}

	virtual void Start() override
	{
	}

	virtual void Update() override 
	{
		for (EntityID ent : m_Entities)
		{
			UnitComponent& unit = m_UnitComponents[ent];
			Transform& transform = m_EntityManager->GetComponent<Transform>(ent);

			if (unit.m_Raised)
			{
				MoveUnit(unit, transform);
				SeizeRegion(unit);
			}
		}
	}

	void MoveUnit(UnitComponent& unit, Transform& transform)
	{
		if (unit.m_Moving)
		{
			if (!transform.m_Position.NearlyEqual(unit.m_Target, m_MoveTolerance))
			{
				Vector2D movement = unit.m_Direction * unit.m_Speed * Time::DeltaTime();
				transform.Translate(movement);
			}
			else
			{
				unit.m_LastPosition = unit.m_Target;
				transform.m_Position = unit.m_Target;
				Vector2DInt pos((int)unit.m_Target.x, (int)unit.m_Target.y);
				Map::m_MapUnitData[pos].AddUnique(unit.m_EntityID);

				if (EnemyAtSquare(pos, m_Warminds[unit.m_Owner].m_Opponent))
				{
					EnterCombat(unit.m_EntityID, m_Warminds[unit.m_EntityID].m_Opponent);
				}

				// Check for enemy at square and kill him
				if (unit.m_CurrentPath.size() > 0)
				{
					Vector2D nextPosition = Map::ConvertToScreen(unit.m_CurrentPath.front());
					unit.m_Target = nextPosition;
					unit.m_Direction = (nextPosition - transform.m_Position).Normalized();

					Vector2DInt mapPos((int)unit.m_LastPosition.x, (int)unit.m_LastPosition.y);
					Map::m_MapUnitData[mapPos].AddUnique(unit.m_EntityID);
					
					unit.m_CurrentPath.pop_front();
				}
				else
				{
					StartConquerRegion(unit, transform);
					// Check if at capital
					unit.m_Moving = false;
				}
			}
		}
	}

	void SeizeRegion(UnitComponent& unit)
	{
		if (unit.m_SeizingRegionID < 0)
		{
			return;
		}

		if (unit.m_LastSeizeDate < Time::m_GameDate.m_Date)
		{
			unit.m_DaysSeizing++;
			unit.m_LastSeizeDate = Time::m_GameDate.m_Date;

			MapRegion region = Map::GetRegionById(unit.m_SeizingRegionID);

			if ((unsigned int)unit.m_DaysSeizing >= region.m_DaysToSeize)
			{
				EntityID loosingEntity = region.m_OwnerID;
				ConquerRegion(unit.m_Owner, loosingEntity, unit.m_SeizingRegionID);
				unit.m_DaysSeizing = 0;
				unit.m_SeizingRegionID = -1;
			}
		}
	}

	void StartConquerRegion(UnitComponent& unit, Transform& transform)
	{
		std::vector<int> regionIDs = Map::GetRegionIDs();
		Vector2DInt currentMapPosition = Map::ConvertToMap(transform.m_Position);

		for each (int regionID in regionIDs)
		{
			Vector2DInt capitalPosition = Map::GetRegionCapitalLocation(regionID);
			if (currentMapPosition == capitalPosition)
			{
				unsigned int ownerID = Map::GetRegionById(regionID).m_OwnerID;
				if (ownerID == unit.m_Owner)
				{
					continue;
				}

				unit.m_SeizingRegionID = regionID;
				unit.m_LastSeizeDate = Time::m_GameDate.m_Date;
			}
		}
	}

	void ConquerRegion(EntityID conqueringID, EntityID loosingEntity, size_t regionID)
	{
#pragma warning(push)
#pragma warning(disable: 26815)
		CharacterSystem* characterSystem = (CharacterSystem*)m_EntityManager->GetSystem<CharacterSystem>().get();
#pragma warning(pop)
		characterSystem->ConquerRegion(regionID, conqueringID);
		characterSystem->LoseRegion(regionID, loosingEntity);
		return;
	}

	bool EnemyAtSquare(Vector2DInt square, EntityID opponent)
	{	
		for (auto& ID : Map::m_MapUnitData[square].m_EntitiesInSquare)
		{
			if (m_UnitComponents[ID].m_Owner == opponent)
			{
 				return true;
			}
		}

		return false;
	}

	void EnterCombat(EntityID unit, EntityID enemyUnit)
	{
		//Very WIP early combat prototype
		//Todo: Needs more weights and values affecting the outcome than army size

		if (m_UnitComponents[unit].m_RepresentedForce > m_UnitComponents[enemyUnit].m_RepresentedForce)
		{
			m_UnitComponents[enemyUnit].m_RepresentedForce = 0;
			m_UnitComponents[enemyUnit].m_Raised = false;
		}

		else
		{
			m_UnitComponents[unit].m_RepresentedForce = 0;
			m_UnitComponents[unit].m_Raised = false;
		}
	}
};
