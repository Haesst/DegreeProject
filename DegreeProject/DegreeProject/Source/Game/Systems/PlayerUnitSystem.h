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
	sf::RectangleShape m_TargetPosition;
	sf::RectangleShape m_EndPosition;
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
			Vector2DInt startPositionMap = MapInfo::GetRegionPositions(0)[0];
			Vector2D startPositionScreen = MapInfo::ConvertToScreen(startPositionMap);
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
			else
			{
				m_Window->draw(m_EndPosition);
				m_Window->draw(m_TargetPosition);
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
			int x = (int)(transform->m_Position.x - 100 + 16) / 32;
			int y = (int)(transform->m_Position.y - 100 + 16) / 32;
			Vector2D startPosition((float)x, (float)y);
			m_Path = Pathfinding::FindPath(Vector2DInt(x, y), InputHandler::GetMouseMapPosition());
			if (m_Path.size() > 0)
			{
				Vector2D firstPosition = Vector2D((float)m_Path.front().x, (float)m_Path.front().y);
				playerUnit->m_Target = firstPosition;
				playerUnit->m_Direction = (firstPosition - startPosition).Normalized();
				playerUnit->m_Moving = true;

				m_Path.pop_front();

				//Debug Start Rectangle
				m_StartPosition.setSize(sf::Vector2f(32.0f, 32.0f));
				m_StartPosition.setPosition(firstPosition.x * 32 + 100 - 16, firstPosition.y * 32 + 100 - 16);
				m_StartPosition.setFillColor(sf::Color::Magenta);

				//Debug End Rectangle
				m_EndPosition.setSize(sf::Vector2f(32.0f, 32.0f));
				Vector2D lastPosition = Vector2D((float)m_Path.back().x, (float)m_Path.back().y);
				m_EndPosition.setPosition(lastPosition.x * 32 + 100 - 16, lastPosition.y * 32 + 100 - 16);
				m_EndPosition.setFillColor(sf::Color::Black);
			}
		}
		if (playerUnit->m_Moving == true)
		{
			playerUnit->m_CurrentMapPosition = Vector2DInt((int)(transform->m_Position.x - 100 + 16) / 32, (int)(transform->m_Position.y - 100 + 16) / 32);
			int x = (int)(transform->m_Position.x - 100 + 16) / 32;
			int y = (int)(transform->m_Position.y - 100 + 16) / 32;
			Vector2DInt targetPos = { (int)playerUnit->m_Target.x, (int)playerUnit->m_Target.y };
			Vector2DInt currentPos = { x,y };

			//Debug Target Rectangle
			m_TargetPosition.setSize(sf::Vector2f(32.0f, 32.0f));
			m_TargetPosition.setPosition((float)targetPos.x * 32 + 100 - 16, (float)targetPos.y * 32 + 100 - 16);
			m_TargetPosition.setFillColor(sf::Color::White);

			if (targetPos != currentPos)
			{
				Vector2D movement = playerUnit->m_Direction * playerUnit->m_Speed * Time::DeltaTime();
				transform->Translate(movement);
				playerUnit->m_Shape.setPosition(transform->m_Position.x, transform->m_Position.y);
			}
			else
			{
				if (m_Path.size() != 0)
				{
					Vector2D nextPosition = Vector2D((float)m_Path.front().x, (float)m_Path.front().y);
					Vector2D startPosition((float)x, (float)y);
					playerUnit->m_Target = nextPosition;
					playerUnit->m_Direction = (nextPosition - startPosition).Normalized();
					m_Path.pop_front();
				}
				else
				{
					playerUnit->m_Moving = false;
					
					//Debug Start Rectangle
					m_StartPosition.setSize(sf::Vector2f(32.0f, 32.0f));
					m_StartPosition.setPosition((float)playerUnit->m_CurrentMapPosition.x * 32 + 100 - 16, (float)playerUnit->m_CurrentMapPosition.y * 32 + 100 - 16);
					m_StartPosition.setFillColor(sf::Color::Magenta);
				}
			}
		}
	}

	void ConquerRegion(PlayerUnit* playerUnit)
	{
		if (!playerUnit->m_Moving && playerUnit->m_CurrentMapPosition != playerUnit->m_RecentlyConquered)
		{
			std::vector<Vector2DInt> regionCapitals = MapInfo::GetRegionCapitals();
			CharacterComponent* characterComponents = m_EntityManager->GetComponentArray<CharacterComponent>();
			Map* mapComponents = m_EntityManager->GetComponentArray<Map>();
			UIText* textComponents = m_EntityManager->GetComponentArray<UIText>();
			unsigned int capitalIndex = 0;
			for each (Vector2DInt capitalPosition in regionCapitals)
			{
				if (playerUnit->m_CurrentMapPosition == capitalPosition)
				{
					unsigned int opponentIndex;
					for (opponentIndex = 19; opponentIndex < 24; opponentIndex++)
					{
						if (opponentIndex == playerUnit->m_Owner)
						{
							continue;
						}
						auto iterator = std::find(characterComponents[opponentIndex].m_OwnedRegionIDs.begin(), characterComponents[opponentIndex].m_OwnedRegionIDs.end(), capitalIndex);
						if (iterator != characterComponents[opponentIndex].m_OwnedRegionIDs.end() && opponentIndex != playerUnit->m_Owner)
						{
							break;
						}
					}
					//LOG_INFO("{0} conquering region {1} from {2}", characterComponents[playerUnit->m_Owner].m_KingdomName, capitalIndex, characterComponents[opponentIndex].m_KingdomName);
					playerUnit->m_RecentlyConquered = capitalPosition;
					characterComponents[playerUnit->m_Owner].m_OwnedRegionIDs.push_back(capitalIndex);
					characterComponents[playerUnit->m_Owner].m_UpdateOwnership = true;
					mapComponents[0].SetRegionColor(capitalIndex, characterComponents[playerUnit->m_Owner].m_RegionColor);
					textComponents[11 + playerUnit->m_Owner].m_OwnedRegions.push_back(capitalIndex);
					textComponents[11 + playerUnit->m_Owner].m_AdjustText = true;
					if (characterComponents[opponentIndex].m_OwnedRegionIDs.size() > 0 && textComponents[opponentIndex + 11].m_OwnedRegions.size() > 0)
					{
						characterComponents[opponentIndex].m_OwnedRegionIDs.erase(std::remove(characterComponents[opponentIndex].m_OwnedRegionIDs.begin(), characterComponents[opponentIndex].m_OwnedRegionIDs.end(), capitalIndex), characterComponents[opponentIndex].m_OwnedRegionIDs.end());
						textComponents[opponentIndex + 11].m_OwnedRegions.erase(std::remove(textComponents[opponentIndex + 11].m_OwnedRegions.begin(), textComponents[opponentIndex + 11].m_OwnedRegions.end(), capitalIndex), textComponents[opponentIndex + 11].m_OwnedRegions.end());
						textComponents[opponentIndex + 11].m_AdjustText = true;
					}
					break;
				}
				capitalIndex++;
			}
		}
	}
};