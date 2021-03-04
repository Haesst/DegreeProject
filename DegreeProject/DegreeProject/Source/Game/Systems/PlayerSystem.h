#pragma once

#include "../../ECS/EntityManager.h"
#include "../../ECS/System.h"

#include "../../Engine/Time.h"
#include "../../Engine/Window.h"

#include "../../ECS/Components/Transform.h"
#include "../Components/Player.h";
#include "Engine/InputHandler.h"

struct PlayerSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	float m_Tolerance = 0.3f;

	// Constructor, Runs when the system is initialized
	// Do any kind of init here but remember to register
	// the systems component signature. IE: add every component
	// that the system needs or should look for
	PlayerSystem()
	{
		AddComponentSignature<Player>();
		m_EntityManager = &EntityManager::Get();
	}

	// Update gets called every frame and loops through every entity that has the signature that
	// the system has registered and do the necessary update
	virtual void Update() override
	{
		Transform* transforms = m_EntityManager->GetComponentArray<Transform>();
		Player* players = m_EntityManager->GetComponentArray<Player>();

		for (auto entity : m_Entities)
		{
			MovePlayer(&transforms[entity], &players[entity]);
			players[entity].m_Shape.setFillColor(players[entity].m_Color);
			players[entity].m_Shape.setSize(sf::Vector2(players[entity].m_Size, players[entity].m_Size));
		}
	}

	virtual void Render() override
	{
		Player* players = m_EntityManager->GetComponentArray<Player>();

		for (auto entity : m_Entities)
		{
			Window::GetWindow()->draw(players[entity].m_Shape);
		}
	}

	void MovePlayer(Transform* transform, Player* player)
	{
		sf::RenderWindow* window = Window::GetWindow();
		
		if (InputHandler::GetRightMouseClicked() == true)
		{
			player->m_Target = InputHandler::GetMousePosition();
			player->m_Direction = InputHandler::GetMousePosition() - transform->m_Position;
			player->m_Direction.Normalized();
		}
		Vector2D movement = player->m_Direction * player->m_Speed * Time::DeltaTime();
		if (transform->m_Position.NearlyEqual(player->m_Target, m_Tolerance))
		{
			movement = movement.Zero();
		}
		transform->Translate(movement);
		player->m_Shape.setPosition(transform->m_Position.x, transform->m_Position.y);
	}
};