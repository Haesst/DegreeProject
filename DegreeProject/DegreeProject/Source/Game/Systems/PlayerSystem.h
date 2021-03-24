#pragma once

#include "ECS/System.h"
#include "Game/Components/Player.h"
#include "Game/Components/Unit.h"

struct PlayerSystem : System
{
	EntityManager* m_EntityManager;

	bool m_Draging = false;
	Vector2D m_MousePosition;
	sf::RectangleShape m_DragWindow;
	float m_ClickTolerance = 16.0f;

	//Debug
	std::vector<sf::RectangleShape> m_DragPositions;
	bool m_Debug = false;

	PlayerSystem()
	{
		AddComponentSignature<Player>();
		m_EntityManager = &EntityManager::Get();
	}

	virtual void Start() override
	{
		Player* playerComponents = m_EntityManager->GetComponentArray<Player>();

		for (auto& entity : m_Entities)
		{
			CharacterComponent& character = m_EntityManager->GetComponent<CharacterComponent>(playerComponents[entity].m_OwnedCharacter);
			character.InitUnit();
		}
	}

	virtual void Update() override
	{
		Player* playerComponents = m_EntityManager->GetComponentArray<Player>();

		ClickDrag();

		for (auto& entity : m_Entities)
		{
			SelectUnit(entity, playerComponents[entity].m_OwnedCharacter);
			MoveUnit(playerComponents[entity].m_SelectedUnitID);
			HoverOverRegion(playerComponents[entity]);
		}
	}

	virtual void Render() override
	{
		if (m_Draging)
		{
			Window::GetWindow()->draw(m_DragWindow);

			//Debug
			if (m_Debug)
			{
				for each (sf::RectangleShape rectangle in m_DragPositions)
				{
					Window::GetWindow()->draw(rectangle);
				}
			}
		}
	}

	void MoveUnit(EntityID unitID)
	{
		if (InputHandler::GetRightMouseReleased() == true && InputHandler::GetPlayerUnitSelected() == true)
		{
			Vector2DInt mousePosition = InputHandler::GetMouseMapPosition();
			UnitComponent& unit = m_EntityManager->GetComponent<UnitComponent>(unitID);
			Transform& transform = m_EntityManager->GetComponent<Transform>(unitID);

			unit.m_Moving = false;
			transform.m_Position = unit.m_LastPosition;
			
			Vector2D unitPosition = transform.m_Position;
			Vector2DInt startingPosition = Map::ConvertToMap(unitPosition);
			std::list<Vector2DInt> path = Pathfinding::FindPath(startingPosition, mousePosition);

			if (path.size() > 0)
			{
				if (path.back() == unit.m_CurrentPath.back())
				{
					unit.m_Moving = true;
					return;
				}

				unit.SetPath(path, Map::ConvertToScreen(startingPosition));
#pragma warning(push)
#pragma warning(disable: 26815)
				UnitSystem* unitSystem = (UnitSystem*)m_EntityManager->GetSystem<UnitSystem>().get();
#pragma warning(pop)
				unitSystem->ShowPath(transform, unit);
			}
			else
			{
				unit.m_CurrentPath.clear();
			}
		}
	}

	void ClickDrag()
	{
		if (InputHandler::GetLeftMouseClicked() == true && m_Draging == false)
		{
			m_MousePosition = InputHandler::GetMousePosition();
			if (InputHandler::GetMouseMoved() == true)
			{
				m_Draging = true;
				m_DragWindow.setSize(sf::Vector2f(10.0f, 10.0f));
				m_DragWindow.setPosition(m_MousePosition.x, m_MousePosition.y);
				m_DragWindow.setFillColor(sf::Color::Transparent);
				m_DragWindow.setOutlineThickness(1.0f);
			}
		}
		if (m_Draging == true)
		{
			Vector2D mousePosition = InputHandler::GetMousePosition();
			Vector2D distance = mousePosition - m_MousePosition;
			m_DragWindow.setSize(sf::Vector2f(distance.x, distance.y));
			if (InputHandler::GetLeftMouseReleased() == true)
			{
				m_Draging = false;
			}
		}
	}

	void SelectUnit(EntityID playerID, EntityID ownerID)
	{
		if (InputHandler::GetLeftMouseClicked() == true && m_Draging == false)
		{
			if (Map::m_MapUnitData.find(InputHandler::GetMouseMapPosition()) != Map::m_MapUnitData.end())
			{
				std::list unitIDList = Map::m_MapUnitData[InputHandler::GetMouseMapPosition()].m_EntitiesInSquare;
				if (unitIDList.size() > 0)
				{
					for (EntityID unitID : unitIDList)
					{
						UnitComponent& unit = m_EntityManager->GetComponent<UnitComponent>(unitID);
						Player& player = m_EntityManager->GetComponent<Player>(playerID);
						if (unit.m_Owner == ownerID)
						{
							unit.m_PlayerSelected = true;
							unit.m_OutlineThickness = 1.0f;
							player.m_SelectedUnitID = unitID;
							InputHandler::SetPlayerUnitSelected(true);
							break;
						}
					}
				}
				else
				{
					DeselectUnit(playerID);
				}
			}
			else
			{
				DeselectUnit(playerID);
			}
		}
		if (m_Draging == true)
		{
			Vector2D firstPosition = m_DragWindow.getPosition();
			float width = m_DragWindow.getGlobalBounds().width;
			float height = m_DragWindow.getGlobalBounds().height;
			unsigned int xPositions = (unsigned int)width / 32;
			unsigned int yPositions = (unsigned int)height / 32;
			for (unsigned int y = 0; y < yPositions; y++)
			{
				for (unsigned int x = 0; x < xPositions; x++)
				{
					Vector2D position = Vector2D(firstPosition.x + x * 32, firstPosition.y + y * 32);
					Vector2DInt mapPosition = Map::ConvertToMap(position);
					if (Map::MapSquareDataContainsKey(mapPosition))
					{
						std::list unitIDList = Map::m_MapUnitData[mapPosition].m_EntitiesInSquare;
						for (EntityID unitID : unitIDList)
						{
							UnitComponent& unit = m_EntityManager->GetComponent<UnitComponent>(unitID);
							if (unit.m_Owner == ownerID)
							{
								unit.m_PlayerSelected = true;
								unit.m_OutlineThickness = 1.0f;
								break;
							}
						}
					}
				}
			}
		}
	}

	void DeselectUnit(EntityID playerID)
	{
		Player& player = m_EntityManager->GetComponent<Player>(playerID);
		UnitComponent& unit = m_EntityManager->GetComponent<UnitComponent>(player.m_SelectedUnitID);
		unit.m_PlayerSelected = false;
		unit.m_OutlineThickness = 0.0f;
		player.m_SelectedUnitID = 0;
		InputHandler::SetPlayerUnitSelected(false);
	}

	void SetDebugPositions(std::vector<Vector2DInt> dragPositions)
	{
		m_DragPositions.clear();
		for (Vector2DInt mapPosition : dragPositions)
		{
			sf::RectangleShape rectangle(sf::Vector2f(30.0f, 30.0f));
			Vector2D screenPosition = Map::ConvertToScreen(mapPosition);
			rectangle.setPosition(screenPosition.x + 2, screenPosition.y + 2);
			rectangle.setFillColor(sf::Color::Magenta);
			m_DragPositions.push_back(rectangle);
		}
	}

	void HoverOverRegion(Player& playerComp)
	{
		Vector2DInt currentMousePosition = InputHandler::GetMouseMapPosition();

		if (Map::m_MapUnitData.find(currentMousePosition) != Map::m_MapUnitData.end())
		{
			EntityID regionID = Map::m_MapUnitData[currentMousePosition].m_RegionID;

			if ((unsigned int)playerComp.m_HighlightedRegion != regionID)
			{
				Map::GetRegionById(playerComp.m_HighlightedRegion).m_Highlighted = false;

				Map::GetRegionById(regionID).m_Highlighted = true;
				playerComp.m_HighlightedRegion = regionID;
			}
		}
		else
		{
			if (playerComp.m_HighlightedRegion >= 0)
			{
				Map::GetRegionById(playerComp.m_HighlightedRegion).m_Highlighted = false;
				playerComp.m_HighlightedRegion = -1;
			}
		}
	}
};