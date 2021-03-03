#pragma once

#include "Engine/FileWatcher.h"
#include "Engine/Window.h"
#include "ECS/System.h"
#include "ECS/EntityManager.h"
#include "ECS/Components/Transform.h"
#include "Game/Components/Map.h"

struct MapSystem : public System
{
	EntityManager* m_EntityManager;
	FileWatcher* m_DataWatcher;
	FileWatcher* m_MapWatcher;

	MapSystem()
	{
		AddComponentSignature<Map>();
		m_EntityManager = &EntityManager::Get();

		HotReloader::Get()->SubscribeToFileChange("Assets\\Data\\Regions.json", std::bind(&MapSystem::RegionsChanged, this, std::placeholders::_1, std::placeholders::_2));
		HotReloader::Get()->SubscribeToFileChange("Assets\\Map\\RegionMap.txt", std::bind(&MapSystem::RegionsChanged, this, std::placeholders::_1, std::placeholders::_2));
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

	void RegionsChanged(std::string path, FileStatus fileStatus)
	{
		if (fileStatus != FileStatus::Modified)
		{
			return;
		}

		for (auto& entity : m_Entities)
		{
			Map* map = &m_EntityManager->GetComponent<Map>(entity);
			//mtx.lock();
			map->LoadAllRegions();
			map->LoadMap();
			//mtx.unlock();
		}
	}
};