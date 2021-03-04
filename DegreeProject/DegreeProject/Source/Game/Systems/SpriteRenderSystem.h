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
		AddComponentSignature<SpriteRenderer>();

		m_EntityManager = &EntityManager::Get();
	}

	virtual void Update() override
	{
		Transform* transforms = m_EntityManager->GetComponentArray<Transform>();
		SpriteRenderer* renderers = m_EntityManager->GetComponentArray<SpriteRenderer>();

		for (auto& entity : m_Entities)
		{
			renderers[entity].m_Sprite.setPosition(transforms[entity].m_Position.x, transforms[entity].m_Position.y);
			renderers[entity].m_Sprite.setTexture(renderers[entity].m_Texture);
			renderers[entity].UpdateSize();
		}
	}

	virtual void Render() override
	{
		SpriteRenderer* renderers = m_EntityManager->GetComponentArray<SpriteRenderer>();
		for (auto& entity : m_Entities)
		{
			Window::GetWindow()->draw(renderers[entity].m_Sprite);
		}
	}
};