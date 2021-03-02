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
		for (auto entity : m_Entities)
		{
			Transform* transform = &m_EntityManager->GetComponent<Transform>(entity);
			Player* player = &m_EntityManager->GetComponent<Player>(entity);

			MovePlayer(transform, player);
			player->m_Shape.setFillColor(player->m_Color);
			player->m_Shape.setSize(sf::Vector2(player->m_Size, player->m_Size));
		}
	}

	virtual void Render() override
	{
		for (auto entity : m_Entities)
		{
			Player* player = &m_EntityManager->GetComponent<Player>(entity);
			Window::GetWindow()->draw(player->m_Shape);
		}
	}

	// Custom method :)
	void MovePlayer(Transform* transform, Player* player)
	{
		sf::RenderWindow* window = Window::GetWindow();
		
		if (InputHandler::GetLeftMouseClicked() == true)
		{
			player->m_Target = InputHandler::GetMousePosition();
			player->m_Direction = InputHandler::GetMousePosition() - transform->m_Position;
			player->m_Direction.Normalized();
		}
		Vector2D movement = player->m_Direction * player->m_Speed * Time::DeltaTime();
		if (transform->m_Position.NearlyEqual(player->m_Target, 1.0f))
		{
			movement = movement.Zero();
		}
		transform->Translate(movement);

		if (transform->m_Position.x > window->getSize().x - 20)
		{
			transform->m_Position.x = 0;
		}
		if (transform->m_Position.y > window->getSize().y - 20)
		{
			transform->m_Position.y = 0;
		}

		player->m_Shape.setPosition(transform->m_Position.x, transform->m_Position.y);
	}
};