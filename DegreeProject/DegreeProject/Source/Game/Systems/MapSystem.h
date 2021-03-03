#pragma once

#include "Engine/Window.h"
#include "ECS/System.h"
#include "ECS/EntityManager.h"
#include "Game/Components/Map.h"
#include "ECS/Components/Transform.h"

struct MapSystem : public System
{
	EntityManager* m_EntityManager;

	MapSystem()
	{
		AddComponentSignature<Map>();

		m_EntityManager = &EntityManager::Get();
	}

	virtual void Update() override
	{
		for (auto& entity : m_Entities)
		{
			Map* map = &m_EntityManager->GetComponent<Map>(entity);

			for (auto& region : map->m_Regions)
			{
				for (auto& square : region.m_MapSquares)
				{
				}
			}
		}
	}

	virtual void Render() override
	{
		for (auto& entity : m_Entities)
		{
			Map* map = &m_EntityManager->GetComponent<Map>(entity);

			for (auto& region : map->m_Regions)
			{
				for (auto& square : region.m_MapSquares)
				{
					sf::Vector2 resolution = Window::GetWindow()->getSize();
					map->m_LandSprite.setTexture(map->m_LandTexture);
					map->m_LandSprite.setTextureRect({ 0,0,32,32 });
					map->m_LandSprite.setColor(region.m_HighlightColor);
					float spriteWidth = map->m_LandSprite.getLocalBounds().width;
					float spriteHeight = map->m_LandSprite.getLocalBounds().height;
					map->m_LandSprite.setPosition(
						resolution.x * 0.1f + (square.x + map->m_XOffset) * spriteWidth * map->m_MapScale,
						(square.y + map->m_YOffset) * spriteHeight * map->m_MapScale);
					Window::GetWindow()->draw(map->m_LandSprite);
				}
			}
		}
	}
};