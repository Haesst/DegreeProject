#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"

#include "Engine/Time.h"
#include "Engine/Window.h"

#include "ECS/Components/Transform.h"
#include "Game/Components/MovingSprite.h"

struct ECSExampleSystem : System
{
	EntityManager* m_EntityManager = nullptr;

	// Constructor, Runs when the system is initialized
	// Do any kind of init here but remember to register
	// the systems component signature. IE: add every component
	// that the system needs or should look for
	ECSExampleSystem()
	{
		addComponentSignature<MovingSprite>();
		m_EntityManager = &EntityManager::get();
	}

	// Update gets called every frame and loops through every entity that has the signature that
	// the system has registered and do the necessary update
	virtual void Update() override
	{
		Transform* transforms = m_EntityManager->getComponentArray<Transform>();
		MovingSprite* movingSprites = m_EntityManager->getComponentArray<MovingSprite>();

		for (auto entity : m_Entities)
		{
			moveSprite(&transforms[entity], &movingSprites[entity]);
			movingSprites[entity].m_Shape.setFillColor(movingSprites[entity].m_Color);
			movingSprites[entity].m_Shape.setRadius(movingSprites[entity].m_Size);
		}
	}

	// Render gets called during the engines render call. This is used to draw stuff.
	// Both Render() and Update() can be omitted.
	virtual void render() override
	{}

	// Custom method :)
	void moveSprite(Transform* transform, MovingSprite* movingSprite)
	{
		sf::RenderWindow* window = Window::getWindow();
		Vector2D movement = movingSprite->m_Direction * movingSprite->m_Speed * Time::deltaTime();
		transform->translate(movement);

		if (transform->m_Position.x > window->getSize().x - 20)
		{
			transform->m_Position.x = 0;
		}
		if (transform->m_Position.y > window->getSize().y - 20)
		{
			transform->m_Position.y = 0;
		}

		movingSprite->m_Shape.setPosition(transform->m_Position.x, transform->m_Position.y);
	}
};