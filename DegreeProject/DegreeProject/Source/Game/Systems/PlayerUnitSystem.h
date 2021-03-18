#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Engine/Time.h"
#include "Engine/Window.h"
#include "ECS/Components/Transform.h"
#include "Game/Components/PlayerUnit.h"
#include "Engine/InputHandler.h"
#include "Game/Pathfinding.h"
#include "Game/Components/UIText.h"

struct PlayerUnitSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	float m_MoveTolerance = 2.0f;
	float m_ClickTolerance = 16.0f;
	sf::RenderWindow* m_Window;
	sf::RectangleShape m_DragWindow;
	sf::RectangleShape m_StartPosition;
	sf::RectangleShape m_EndPosition;
	std::vector<sf::RectangleShape> m_TargetPath;
	bool m_Draging = false;
	Vector2D m_MousePosition;
	std::list<Vector2DInt> m_Path;

	// Constructor, Runs when the system is initialized
	// Do any kind of init here but remember to register
	// the systems component signature. IE: add every component
	// that the system needs or should look for
	PlayerUnitSystem()
	{
		AddComponentSignature<PlayerUnit>();
		m_EntityManager = &EntityManager::Get();
		m_Window = Window::GetWindow();
	}

	virtual void Start() override
	{
		Pathfinding::Init(Map::m_Data.m_Regions);

		Transform* transforms = m_EntityManager->GetComponentArray<Transform>();
		PlayerUnit* playerUnits = m_EntityManager->GetComponentArray<PlayerUnit>();

		for (auto entity : m_Entities)
		{
			Vector2DInt startPositionMap = Map::m_Data.m_Regions[0].m_MapSquares[0];
			Vector2D startPositionScreen = Map::ConvertToScreen(startPositionMap);
			transforms[entity].m_Position = startPositionScreen;
			playerUnits[entity].m_CurrentMapPosition = startPositionMap;
			playerUnits[entity].m_RecentlyConquered = startPositionMap;
			playerUnits[entity].m_Shape.setPosition(startPositionScreen.x, startPositionScreen.y);

			//Debug Start Rectangle
			m_StartPosition.setSize(sf::Vector2f(32.0f, 32.0f));
			m_StartPosition.setPosition(startPositionScreen.x, startPositionScreen.y);
			m_StartPosition.setFillColor(sf::Color::Magenta);
		}
	}

	// Update gets called every frame and loops through every entity that has the signature that
	// the system has registered and do the necessary update
	virtual void Update() override
	{
		Transform* transforms = m_EntityManager->GetComponentArray<Transform>();
		PlayerUnit* playerUnits = m_EntityManager->GetComponentArray<PlayerUnit>();

		InputHandler::SetPlayerUnitSelected(false);

		ClickDrag();

		for (auto entity : m_Entities)
		{
			SelectPlayer(&transforms[entity], &playerUnits[entity]);
			MovePlayer(&transforms[entity], &playerUnits[entity]);
			ConquerRegion(&playerUnits[entity]);
			playerUnits[entity].m_Shape.setFillColor(playerUnits[entity].m_FillColor);
			playerUnits[entity].m_Shape.setOutlineColor(playerUnits[entity].m_OutlineColor);
			playerUnits[entity].m_Shape.setOutlineThickness(playerUnits[entity].m_OutlineThickness);
			playerUnits[entity].m_Shape.setSize(sf::Vector2(playerUnits[entity].m_Size, playerUnits[entity].m_Size));
			if (playerUnits[entity].m_Selected == true)
			{
				InputHandler::SetPlayerUnitSelected(true);
			}
		}
	}

	virtual void Render() override
	{
		PlayerUnit* playerUnits = m_EntityManager->GetComponentArray<PlayerUnit>();

		for (auto entity : m_Entities)
		{
			m_Window->draw(playerUnits[entity].m_Shape);
			if (m_Draging)
			{
				m_Window->draw(m_DragWindow);
			}
			if(playerUnits[entity].m_Moving)
			{
				for each (sf::RectangleShape rectangle in m_TargetPath)
				{
					m_Window->draw(rectangle);
				}
				m_Window->draw(m_EndPosition);
				m_Window->draw(m_StartPosition);
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
		}
		if (InputHandler::GetLeftMouseReleased() == true)
		{
			m_Draging = false;
		}
	}

	void SelectPlayer(Transform* transform, PlayerUnit* playerUnit)
	{
		if (InputHandler::GetLeftMouseClicked() == true && m_Draging == false)
		{
			if (transform->m_Position.NearlyEqual(m_MousePosition, m_ClickTolerance, playerUnit->m_Size * 0.5f, playerUnit->m_Size * 0.5f))
			{
				playerUnit->m_Selected = true;
				playerUnit->m_OutlineThickness = 1.0f;
			}
			else
			{
				playerUnit->m_Selected = false;
				playerUnit->m_OutlineThickness = 0.0f;
			}
		}
		if (m_Draging == true)
		{
			if (m_DragWindow.getGlobalBounds().contains(transform->m_Position.x + playerUnit->m_Size * 0.5f, transform->m_Position.y + playerUnit->m_Size * 0.5f))
			{
				playerUnit->m_Selected = true;
				playerUnit->m_OutlineThickness = 1.0f;
			}
			else
			{
				playerUnit->m_Selected = false;
				playerUnit->m_OutlineThickness = 0.0f;
			}
		}
	}

	void ShowPath(Transform* transform, PlayerUnit* playerUnit)
	{
		std::vector<Vector2DInt> path;
		for each (Vector2DInt position in m_Path)
		{
			path.push_back(position);
		}
		unsigned int index = 0;
		for each (Vector2DInt position in path)
		{
			Vector2D screenPosition = Map::ConvertToScreen(position);
			Vector2D rectangleSize = Vector2D(playerUnit->m_Shape.getSize().x * 1.45f, playerUnit->m_Shape.getSize().y * 0.25f);
			float rotation = 0.0f;
			Vector2DInt oldPosition = Vector2DInt();
			if (m_TargetPath.size() > 0)
			{
				oldPosition = path[index - 1];
			}
			else
			{
				oldPosition = Map::ConvertToMap(transform->m_Position);
			}
			Vector2DInt change = position - oldPosition;
			if (abs(change.y) == 1 && change.x == 0)
			{
				rotation = 90.0f;
				screenPosition.x += playerUnit->m_Shape.getSize().x * 0.625f;
				screenPosition.y -= playerUnit->m_Shape.getSize().y * 0.375f;
				if (change.y < 0)
				{
					screenPosition.y += playerUnit->m_Shape.getSize().y * 0.5f;
				}
				else
				{
					screenPosition.y -= playerUnit->m_Shape.getSize().y * 0.5f;
				}
			}
			else if (change.x > 0 && change.y > 0 || change.x < 0 && change.y < 0)
			{
				rotation = 45.0f;
				screenPosition.x += playerUnit->m_Shape.getSize().x * 0.28125f;
				screenPosition.y -= playerUnit->m_Shape.getSize().y * 0.28125f;
				if (change.x > 0 && change.y > 0)
				{
					screenPosition.x -= playerUnit->m_Shape.getSize().x;
					screenPosition.y -= playerUnit->m_Shape.getSize().y;
				}
				if (change.x < 0 && change.y < 0)
				{
					screenPosition.x += playerUnit->m_Shape.getSize().x * 0.5f;
					screenPosition.y += playerUnit->m_Shape.getSize().y * 0.5f;
				}
			}
			else if (change.x > 0 && change.y < 0 || change.x < 0 && change.y > 0)
			{
				rotation = -45.0f;
				screenPosition.x += playerUnit->m_Shape.getSize().x * 0.21875f;
				screenPosition.y += playerUnit->m_Shape.getSize().y * 0.21875f;
				if (change.x > 0 && change.y < 0)
				{
					screenPosition.x -= playerUnit->m_Shape.getSize().x;
					screenPosition.y += playerUnit->m_Shape.getSize().y;
				}
			}
			sf::RectangleShape rectangle = sf::RectangleShape(sf::Vector2f(rectangleSize.x, rectangleSize.y));
			rectangle.setFillColor(sf::Color::White);
			rectangle.setRotation(rotation);
			rectangle.setPosition(screenPosition.x, screenPosition.y + playerUnit->m_Shape.getSize().y * 0.375f);
			m_TargetPath.push_back(rectangle);
			index++;
		}
	}

	void MovePlayer(Transform* transform, PlayerUnit* playerUnit)
	{
		if (InputHandler::GetRightMouseReleased() == true && playerUnit->m_Selected == true)
		{
			m_Path.clear();
			m_TargetPath.clear();
			Vector2DInt startPositionMap = Map::ConvertToMap(transform->m_Position);
			Vector2D startPositionFloat = Vector2D((float)startPositionMap.x, (float)startPositionMap.y);
			m_Path = Pathfinding::FindPath(startPositionMap, InputHandler::GetMouseMapPosition());

			ShowPath(transform, playerUnit);

			if (m_Path.size() > 0)
			{
				Vector2DInt firstPositionMap = m_Path.front();
				Vector2D firstPositionFloat = Vector2D((float)firstPositionMap.x, (float)firstPositionMap.y);
				playerUnit->m_Target = Map::ConvertToScreen(firstPositionMap);
				playerUnit->m_Direction = (playerUnit->m_Target - transform->m_Position).Normalized();
				playerUnit->m_Moving = true;

				m_Path.pop_front();

				//Debug Start Rectangle
				m_StartPosition.setSize(sf::Vector2f(32.0f, 32.0f));
				Vector2D rectanglePosition = Map::ConvertToScreen(playerUnit->m_CurrentMapPosition);
				m_StartPosition.setPosition(rectanglePosition.x, rectanglePosition.y);
				m_StartPosition.setFillColor(sf::Color::Magenta);

				//Debug End Rectangle
				m_EndPosition.setSize(sf::Vector2f(32.0f, 32.0f));
				Vector2D lastPosition = Map::ConvertToScreen(m_Path.back());
				m_EndPosition.setPosition(lastPosition.x, lastPosition.y);
				m_EndPosition.setFillColor(sf::Color::Black);
			}
		}
		if (playerUnit->m_Moving == true)
		{
			if (!transform->m_Position.NearlyEqual(playerUnit->m_Target, 0.2f))
			{
				Vector2D movement = playerUnit->m_Direction * playerUnit->m_Speed * Time::DeltaTime();
				transform->Translate(movement);
				playerUnit->m_Shape.setPosition(transform->m_Position.x, transform->m_Position.y);
			}
			else
			{
				transform->m_Position = playerUnit->m_Target;
				playerUnit->m_CurrentMapPosition = Map::ConvertToMap(transform->m_Position);

				if (m_Path.size() > 0)
				{
					Vector2D nextPosition = Map::ConvertToScreen(m_Path.front());
					playerUnit->m_Target = nextPosition;
					playerUnit->m_Direction = (nextPosition - transform->m_Position).Normalized();

					m_Path.pop_front();

					m_TargetPath.erase(m_TargetPath.begin());
				}
				else
				{
					playerUnit->m_Moving = false;

					m_TargetPath.clear();
					m_StartPosition.setFillColor(sf::Color::Transparent);
					m_EndPosition.setFillColor(sf::Color::Transparent);
				}
			}
		}
	}

	void ConquerRegion(PlayerUnit* playerUnit)
	{
		if (!playerUnit->m_Moving && playerUnit->m_CurrentMapPosition != playerUnit->m_RecentlyConquered)
		{	
			std::vector<int> regionIDs = Map::GetRegionIDs();
			for each (int regionID in regionIDs)
			{
				Vector2DInt capitalPosition = Map::GetRegionCapitalLocation(regionID);
				if (playerUnit->m_CurrentMapPosition == capitalPosition)
				{
					unsigned int ownerID = Map::GetRegionById(regionID).m_OwnerID;
					if (ownerID == playerUnit->m_Owner)
					{
						playerUnit->m_RecentlyConquered = capitalPosition;
						continue;
					}
					playerUnit->m_RecentlyConquered = capitalPosition;
#pragma warning(push)
#pragma warning(disable: 26815)
					CharacterSystem* characterSystem = (CharacterSystem*)m_EntityManager->GetSystem<CharacterSystem>().get();
#pragma warning(pop)
					characterSystem->ConquerRegion(regionID, playerUnit->m_Owner);
					characterSystem->LoseRegion(regionID, ownerID);
					return;
				}
			}
		}
	}
};