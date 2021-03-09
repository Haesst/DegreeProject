#pragma once

#include "Engine/Window.h"
#include "ECS/System.h"
#include "Game/Components/UISpriteRenderer.h"
#include "ECS/Components/Transform.h"

struct UISpriteRenderSystem : public System
{
	EntityManager* m_EntityManager;

	UISpriteRenderSystem()
	{
		AddComponentSignature<UISpriteRenderer>();

		m_EntityManager = &EntityManager::Get();
	}

	virtual void Update() override
	{
		Transform* transforms = m_EntityManager->GetComponentArray<Transform>();
		UISpriteRenderer* renderers = m_EntityManager->GetComponentArray<UISpriteRenderer>();

		for (auto& entity : m_Entities)
		{
			renderers[entity].m_Sprite.setPosition(transforms[entity].m_Position.x, transforms[entity].m_Position.y);
			renderers[entity].m_Sprite.setTexture(renderers[entity].m_Texture);
			renderers[entity].UpdateSize();
		}
	}

	virtual void Render() override
	{
		UISpriteRenderer* renderers = m_EntityManager->GetComponentArray<UISpriteRenderer>();
		for (auto& entity : m_Entities)
		{
			Window::GetWindow()->draw(renderers[entity].m_Sprite);
		}
	}
};