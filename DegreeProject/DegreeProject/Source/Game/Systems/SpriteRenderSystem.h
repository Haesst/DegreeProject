#pragma once

#include "Engine/Window.h"
#include "ECS/System.h"
#include "Game/Components/SpriteRenderer.h"
#include "ECS/Components/Transform.h"

struct SpriteRenderSystem : public System
{
	EntityManager* m_EntityManager;

	SpriteRenderSystem()
	{
		addComponentSignature<SpriteRenderer>();

		m_EntityManager = &EntityManager::get();
	}

	virtual void update() override
	{
		Transform* transforms = m_EntityManager->getComponentArray<Transform>();
		SpriteRenderer* renderers = m_EntityManager->getComponentArray<SpriteRenderer>();

		for (auto& entity : m_Entities)
		{
			renderers[entity].m_Sprite.setPosition(transforms[entity].m_Position.x, transforms[entity].m_Position.y);
			renderers[entity].m_Sprite.setTexture(renderers[entity].m_Texture);
			renderers[entity].updateSize();
		}
	}

	virtual void render() override
	{
		SpriteRenderer* renderers = m_EntityManager->getComponentArray<SpriteRenderer>();
		for (auto& entity : m_Entities)
		{
			if (renderers[entity].m_ShouldRender)
			{
				Window::getWindow()->draw(renderers[entity].m_Sprite);
			}
		}
	}
};