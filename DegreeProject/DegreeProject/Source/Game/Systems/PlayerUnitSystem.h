#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Engine/Time.h"
#include "Engine/Window.h"
#include "ECS/Components/Transform.h"
#include "Game/Components/PlayerUnit.h"
#include "Engine/InputHandler.h"
#include "Game/Pathfinding.h"

struct PlayerUnitSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	float m_MoveTolerance = 2.0f;
	float m_ClickTolerance = 16.0f;
	sf::RenderWindow* m_Window;
	sf::RectangleShape m_DragWindow;
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
		Pathfinding::Init(MapInfo::GetMapRegions());

		Transform* transforms = m_EntityManager->GetComponentArray<Transform>();
		PlayerUnit* playerUnits = m_EntityManager->GetComponentArray<PlayerUnit>();

		for (auto entity : m_Entities)
		{
			Vector2DInt startPosition = MapInfo::GetRegionPositions(0)[0];
			transforms[entity].m_Position = Vector2D(startPosition.x, startPosition.y);
			playerUnits[entity].m_Shape.setPosition(startPosition.x, startPosition.y);
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
			if (transform->m_Position.NearlyEqual(m_MousePosition, m_ClickTolerance,  playerUnit->m_Size * 0.5f, playerUnit->m_Size * 0.5f))
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

	void MovePlayer(Transform* transform, PlayerUnit* playerUnit)
	{
		if (InputHandler::GetRightMouseReleased() == true && playerUnit->m_Selected == true)
		{
			m_Path.clear();
			int x = (transform->m_Position.x - 100 + 16) / 32;
			int y = (transform->m_Position.y - 100 + 16) / 32;
			Vector2D startPosition(x, y);
			m_Path = Pathfinding::FindPath(Vector2DInt(x, y), InputHandler::GetMouseMapPosition());
			if (m_Path.size() > 0)
			{
				Vector2D firstPosition = Vector2D(m_Path.front().x, m_Path.front().y);
				playerUnit->m_Target = firstPosition;
				playerUnit->m_Direction = (firstPosition - startPosition).Normalized();
				playerUnit->m_Moving = true;

				m_Path.pop_front();
			}
			//for each (Vector2DInt position in m_Path)
			//{
			//	LOG_INFO("{0}", position);
			//}
		}
		if (playerUnit->m_Moving == true)
		{
			int x = (transform->m_Position.x - 100 + 16) / 32;
			int y = (transform->m_Position.y - 100 + 16) / 32;
			Vector2DInt targetPos = { (int)playerUnit->m_Target.x, (int)playerUnit->m_Target.y };
			Vector2DInt currentPos = { x,y };
			if (targetPos != currentPos)
			{
				Vector2D movement = playerUnit->m_Direction * playerUnit->m_Speed * Time::DeltaTime();
				transform->Translate(movement);
				playerUnit->m_Shape.setPosition(transform->m_Position.x, transform->m_Position.y);
			}
			else
			{
				//transform->m_Position = playerUnit->m_Target;
				if (m_Path.size() != 0)
				{
					Vector2D nextPosition = Vector2D(m_Path.front().x, m_Path.front().y);
					Vector2D startPosition(x, y);
					playerUnit->m_Target = nextPosition;
					playerUnit->m_Direction = (nextPosition - startPosition).Normalized();
					m_Path.pop_front();
				}
				else
				{
					playerUnit->m_Moving = false;
				}
			}
		}
	}
};