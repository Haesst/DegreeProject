#pragma once

#include "../../ECS/EntityManager.h"
#include "../../ECS/System.h"

#include "../../Engine/Time.h"
#include "../../Engine/Window.h"

#include "../../ECS/Components/Transform.h"
#include "../Components/MovingCircle.h";

struct ECSExampleSystem : System
{
	EntityManager* m_EntityManager = nullptr;

	// Constructor, Runs when the system is initialized
	// Do any kind of init here but remember to register
	// the systems component signature. IE: add every component
	// that the system needs or should look for
	ECSExampleSystem()
	{
		AddComponentSignature<MovingCircle>();
		AddComponentSignature<Transform>();
		m_EntityManager = &EntityManager::Get();
	}

	// Update gets called every frame and loops through every entity that has the signature that
	// the system has registred and do the necessary update
	virtual void Update() override
	{
		for (auto entity : m_Entities)
		{
			Transform* transform = &m_EntityManager->GetComponent<Transform>(entity);
			MovingCircle* movingCircle = &m_EntityManager->GetComponent<MovingCircle>(entity);

			MoveCircle(transform, movingCircle);
			movingCircle->m_Shape.setFillColor(movingCircle->m_Color);
			movingCircle->m_Shape.setRadius(movingCircle->m_Size);
		}
	}

	virtual void Render() override
	{
		for (auto entity : m_Entities)
		{
			MovingCircle* movingCircle = &m_EntityManager->GetComponent<MovingCircle>(entity);
			Window::GetWindow()->draw(movingCircle->m_Shape);
		}
	}

	// Custom method :)
	void MoveCircle(Transform* transform, MovingCircle* movingCircle)
	{
		sf::RenderWindow* window = Window::GetWindow();
		Vector2D movement = movingCircle->m_Direction * movingCircle->m_Speed * Time::DeltaTime();
		transform->Translate(movement);

		if (transform->m_Position.x > window->getSize().x - 20)
		{
			transform->m_Position.x = 0;
		}
		if (transform->m_Position.y > window->getSize().y - 20)
		{
			transform->m_Position.y = 0;
		}

		movingCircle->m_Shape.setPosition(transform->m_Position.x, transform->m_Position.y);
	}
};