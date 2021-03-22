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
	SpriteRenderer* m_Renderers = nullptr;

	std::vector<sf::RectangleShape> m_TargetPath;

	float m_MoveTolerance = 0.3f;

	UnitSystem()
	{
		AddComponentSignature<UnitComponent>();
		m_EntityManager = &EntityManager::Get();
		m_UnitComponents = m_EntityManager->GetComponentArray<UnitComponent>();
		m_Warminds = m_EntityManager->GetComponentArray<WarmindComponent>();
		m_Characters = m_EntityManager->GetComponentArray<CharacterComponent>();
		m_Renderers = m_EntityManager->GetComponentArray<SpriteRenderer>();
	}

	virtual void Start() override
	{
		for (EntityID entity : m_Entities)
		{
			UnitComponent& unit = m_UnitComponents[entity];
			Transform& transform = m_EntityManager->GetComponent<Transform>(entity);
			unit.m_HighlightShape.setFillColor(unit.m_FillColor);
			unit.m_HighlightShape.setOutlineColor(unit.m_OutlineColor);
			unit.m_HighlightShape.setOutlineThickness(unit.m_OutlineThickness);
			unit.m_HighlightShape.setSize(unit.m_HighlightShapeSize);
			unit.m_HighlightShape.setPosition(transform.m_Position.x, transform.m_Position.y);
		}
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
				unit.m_HighlightShape.setFillColor(unit.m_FillColor);
				unit.m_HighlightShape.setOutlineColor(unit.m_OutlineColor);
				unit.m_HighlightShape.setOutlineThickness(unit.m_OutlineThickness);
				unit.m_HighlightShape.setSize(unit.m_HighlightShapeSize);
				unit.m_HighlightShape.setPosition(transform.m_Position.x, transform.m_Position.y);
			}
		}
	}

	virtual void Render() override
	{
		for (EntityID entity : m_Entities)
		{
			UnitComponent& unit = m_UnitComponents[entity];
			Window::GetWindow()->draw(unit.m_HighlightShape);
			if (unit.m_Moving && unit.m_PlayerSelected)
			{
				for each (sf::RectangleShape rectangle in m_TargetPath)
				{
					Window::GetWindow()->draw(rectangle);
				}
			}
			if (unit.m_Raised)
			{
				sf::RectangleShape shape;
				shape.setSize(sf::Vector2f(32.0f, 32.0f));
				shape.setFillColor(sf::Color::Red);

				Vector2D pos = Map::ConvertToScreen(unit.m_CurrentPath.back());
				shape.setPosition(pos.x, pos.y);
				Window::GetWindow()->draw(shape);
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
				Vector2DInt pos = Map::ConvertToMap(unit.m_Target);
				Map::m_MapUnitData[pos].AddUnique(unit.m_EntityID);

				if (EnemyAtSquare(pos, m_Warminds[unit.m_Owner].m_Opponent))
				{
					EntityID enemyID = m_Warminds[unit.m_Owner].m_Opponent;
					UnitComponent& enemyUnit = m_UnitComponents[m_Warminds[enemyID].m_UnitEntity];
					EnterCombat(unit.m_EntityID, enemyUnit.GetID());
				}

				// Check for enemy at square and kill him
				if (unit.m_CurrentPath.size() > 0)
				{
					Vector2D nextPosition = Map::ConvertToScreen(unit.m_CurrentPath.front());
					unit.m_Target = nextPosition;
					unit.m_Direction = (nextPosition - transform.m_Position).Normalized();

					Vector2DInt mapPos = Map::ConvertToMap(unit.m_LastPosition);
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
			m_Renderers[unit].m_ShouldRender = false;
		}
	}

	void ShowPath(Transform& transform, UnitComponent& unit)
	{
		std::vector<Vector2DInt> path;
		for each (Vector2DInt position in unit.m_CurrentPath)
		{
			path.push_back(position);
		}
		unsigned int index = 0;
		for each (Vector2DInt position in path)
		{
			Vector2D screenPosition = Map::ConvertToScreen(position);
			Vector2D rectangleSize = Vector2D(unit.m_HighlightShape.getSize().x * 1.45f, unit.m_HighlightShape.getSize().y * 0.25f);
			float rotation = 0.0f;
			Vector2DInt oldPosition = Vector2DInt();
			if (m_TargetPath.size() > 0)
			{
				oldPosition = path[index - 1];
			}
			else
			{
				oldPosition = Map::ConvertToMap(transform.m_Position);
			}
			Vector2DInt change = position - oldPosition;
			if (abs(change.y) == 1 && change.x == 0)
			{
				rotation = 90.0f;
				screenPosition.x += unit.m_HighlightShape.getSize().x * 0.625f;
				screenPosition.y -= unit.m_HighlightShape.getSize().y * 0.375f;
				if (change.y < 0)
				{
					screenPosition.y += unit.m_HighlightShape.getSize().y * 0.5f;
				}
				else
				{
					screenPosition.y -= unit.m_HighlightShape.getSize().y * 0.5f;
				}
			}
			else if (change.x > 0 && change.y > 0 || change.x < 0 && change.y < 0)
			{
				rotation = 45.0f;
				screenPosition.x += unit.m_HighlightShape.getSize().x * 0.28125f;
				screenPosition.y -= unit.m_HighlightShape.getSize().y * 0.28125f;
				if (change.x > 0 && change.y > 0)
				{
					screenPosition.x -= unit.m_HighlightShape.getSize().x;
					screenPosition.y -= unit.m_HighlightShape.getSize().y;
				}
				if (change.x < 0 && change.y < 0)
				{
					screenPosition.x += unit.m_HighlightShape.getSize().x * 0.5f;
					screenPosition.y += unit.m_HighlightShape.getSize().y * 0.5f;
				}
			}
			else if (change.x > 0 && change.y < 0 || change.x < 0 && change.y > 0)
			{
				rotation = -45.0f;
				screenPosition.x += unit.m_HighlightShape.getSize().x * 0.21875f;
				screenPosition.y += unit.m_HighlightShape.getSize().y * 0.21875f;
				if (change.x > 0 && change.y < 0)
				{
					screenPosition.x -= unit.m_HighlightShape.getSize().x;
					screenPosition.y += unit.m_HighlightShape.getSize().y;
				}
			}
			sf::RectangleShape rectangle = sf::RectangleShape(sf::Vector2f(rectangleSize.x, rectangleSize.y));
			rectangle.setFillColor(sf::Color::White);
			rectangle.setRotation(rotation);
			rectangle.setPosition(screenPosition.x, screenPosition.y + unit.m_HighlightShape.getSize().y * 0.375f);
			m_TargetPath.push_back(rectangle);
			index++;
		}
	}
};
