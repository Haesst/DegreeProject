#pragma once

#include "Engine/FileWatcher.h"
#include "Engine/Window.h"
#include "ECS/System.h"
#include "Game/Mutex.h"
#include "ECS/EntityManager.h"
#include "ECS/Components/Transform.h"
#include "Game/Components/Map.h"

struct MapSystem : public System
{
	EntityManager* m_EntityManager = nullptr;

	MapSystem()
	{
		AddComponentSignature<Map>();
		m_EntityManager = &EntityManager::Get();

		HotReloader::Get()->SubscribeToFileChange("Assets\\Data\\Regions.json", std::bind(&MapSystem::RegionsChanged, this, std::placeholders::_1, std::placeholders::_2));
		HotReloader::Get()->SubscribeToFileChange("Assets\\Map\\RegionMap.txt", std::bind(&MapSystem::RegionsChanged, this, std::placeholders::_1, std::placeholders::_2));
	}

	virtual void Render() override
	{
		Map* maps = m_EntityManager->GetComponentArray<Map>();
		for (auto& entity : m_Entities)
		{
			for (auto& region : maps[entity].m_Regions)
			{
				for (auto& square : region.m_MapSquares)
				{
					sf::Vector2 resolution = Window::GetWindow()->getSize();
					maps[entity].m_LandSprite.setTexture(maps[entity].m_LandTexture);
					maps[entity].m_LandSprite.setTextureRect({ 0,0,32,32 });
					maps[entity].m_LandSprite.setColor(region.m_HighlightColor);
					float spriteWidth = maps[entity].m_LandSprite.getLocalBounds().width;
					float spriteHeight = maps[entity].m_LandSprite.getLocalBounds().height;
					maps[entity].m_LandSprite.setPosition(
						resolution.x * 0.1f + (square.x + maps[entity].m_XOffset) * spriteWidth * maps[entity].m_MapScale,
						(square.y + maps[entity].m_YOffset) * spriteHeight * maps[entity].m_MapScale);
					Window::GetWindow()->draw(maps[entity].m_LandSprite);
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

		Map* maps = m_EntityManager->GetComponentArray<Map>();
		for (auto& entity : m_Entities)
		{
			mtx.lock();
			maps[entity].LoadAllRegions();
			maps[entity].LoadMap();
			mtx.unlock();
		}
	}
};