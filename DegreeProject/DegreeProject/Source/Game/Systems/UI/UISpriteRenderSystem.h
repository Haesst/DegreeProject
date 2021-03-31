#pragma once

#include "Engine/Window.h"
#include "ECS/System.h"
#include "Game/Components/UI/UISpriteRenderer.h"
#include "ECS/Components/Transform.h"

struct UISpriteRenderSystem : public System
{
	EntityManager* m_EntityManager;

	UISpriteRenderSystem()
	{
		addComponentSignature<UISpriteRenderer>();

		m_EntityManager = &EntityManager::get();
	}

	virtual void update() override
	{
		Transform* transforms = m_EntityManager->getComponentArray<Transform>();
		UISpriteRenderer* renderers = m_EntityManager->getComponentArray<UISpriteRenderer>();

		for (auto& entity : m_Entities)
		{
			renderers[entity].m_Sprite.setPosition(Window::getWindow()->mapPixelToCoords(sf::Vector2i((int)transforms[entity].m_Position.x, (int)transforms[entity].m_Position.y)));
			renderers[entity].m_Sprite.setTexture(renderers[entity].m_Texture);
			renderers[entity].updateSize();
		}
	}

	virtual void render() override
	{
		UISpriteRenderer* renderers = m_EntityManager->getComponentArray<UISpriteRenderer>();
		for (auto& entity : m_Entities)
		{
			Window::getWindow()->draw(renderers[entity].m_Sprite);
		}
	}
};