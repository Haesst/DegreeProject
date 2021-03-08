#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"

#include "Engine/Time.h"
#include "Engine/Window.h"

#include "ECS/Components/Transform.h"
#include "Game/Components/Player.h";
#include "Engine/InputHandler.h"

struct PlayerSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	float m_MoveTolerance = 1.0f;
	float m_ClickTolerance = 5.0f;
	sf::RenderWindow* window;

	// Constructor, Runs when the system is initialized
	// Do any kind of init here but remember to register
	// the systems component signature. IE: add every component
	// that the system needs or should look for
	PlayerSystem()
	{
		AddComponentSignature<Player>();
		m_EntityManager = &EntityManager::Get();
		window = Window::GetWindow();
	}

	// Update gets called every frame and loops through every entity that has the signature that
	// the system has registered and do the necessary update
	virtual void Update() override
	{
		Transform* transforms = m_EntityManager->GetComponentArray<Transform>();
		Player* players = m_EntityManager->GetComponentArray<Player>();

		for (auto entity : m_Entities)
		{
			SelectPlayer(&transforms[entity], &players[entity]);
			MovePlayer(&transforms[entity], &players[entity]);
			players[entity].m_Shape.setFillColor(players[entity].m_FillColor);
			players[entity].m_Shape.setOutlineColor(players[entity].m_OutlineColor);
			players[entity].m_Shape.setOutlineThickness(players[entity].m_OutlineThickness);
			players[entity].m_Shape.setSize(sf::Vector2(players[entity].m_Size, players[entity].m_Size));
		}
	}

	virtual void Render() override
	{
		Player* players = m_EntityManager->GetComponentArray<Player>();

		for (auto entity : m_Entities)
		{
			window->draw(players[entity].m_Shape);
		}
	}

	void SelectPlayer(Transform* transform, Player* player)
	{
		if (InputHandler::GetLeftMouseClicked() == true)
		{
			if (transform->m_Position.NearlyEqual(InputHandler::GetMousePosition(), m_ClickTolerance,  player->m_Shape.getLocalBounds().width * 0.5f, player->m_Shape.getLocalBounds().height * 0.5f))
			{
				player->m_Selected = true;
				InputHandler::SetPlayerSelected(true);
				player->m_OutlineThickness = 1.0f;
			}
			else
			{
				player->m_Selected = false;
				InputHandler::SetPlayerSelected(false);
				player->m_OutlineThickness = 0.0f;
			}
		}
	}

	void MovePlayer(Transform* transform, Player* player)
	{
		if (InputHandler::GetRightMouseClicked() == true && player->m_Selected == true)
		{
			player->m_Target = InputHandler::GetMousePosition();
			Vector2D positionCenter = Vector2D(transform->m_Position.x + player->m_Shape.getLocalBounds().width * 0.5f, transform->m_Position.y + player->m_Shape.getLocalBounds().height * 0.5f);
			player->m_Direction = player->m_Target - positionCenter;
			player->m_Direction.Normalized();
		}
		if (!transform->m_Position.NearlyEqual(player->m_Target, m_MoveTolerance, player->m_Shape.getLocalBounds().width * 0.5f, player->m_Shape.getLocalBounds().height * 0.5f))
		{
			Vector2D movement = player->m_Direction * player->m_Speed * Time::DeltaTime();
			transform->Translate(movement);
			player->m_Shape.setPosition(transform->m_Position.x, transform->m_Position.y);
		}
	}
};