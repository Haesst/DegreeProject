#pragma once

#include "../../Engine/Window.h"
#include "../../ECS/System.h"
#include "../Components/SpriteRenderer.h"
#include "../../ECS/Components/Transform.h"

struct SpriteRenderSystem : public System
{
	EntityManager* m_EntityManager;

	SpriteRenderSystem()
	{
		AddComponentSignature<SpriteRenderer>();

		m_EntityManager = &EntityManager::Get();
	}

	virtual void Update() override
	{
		for (auto& entity : m_Entities)
		{
			Transform* transform = &m_EntityManager->GetComponent<Transform>(entity);
			SpriteRenderer* spriteRenderer = &m_EntityManager->GetComponent<SpriteRenderer>(entity);

			spriteRenderer->m_Sprite.setPosition(transform->m_Position.x, transform->m_Position.y);
			spriteRenderer->m_Sprite.setTexture(spriteRenderer->m_Texture);
			spriteRenderer->UpdateSize();
		}
	}

	virtual void Render() override
	{
		for (auto& entity : m_Entities)
		{
			SpriteRenderer* spriteRenderer = &m_EntityManager->GetComponent<SpriteRenderer>(entity);
			Window::GetWindow()->draw(spriteRenderer->m_Sprite);
		}
	}
};