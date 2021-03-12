#pragma once

#include "Engine/FileWatcher.h"
#include "Engine/Window.h"
#include "ECS/System.h"
#include "Game/Mutex.h"
#include "ECS/EntityManager.h"
#include "ECS/Components/Transform.h"
#include "Game/Components/Map.h"
#include "Engine/Vector2D.h"
#include <iostream>
#include <mutex>

struct MapSystem : public System
{
	EntityManager* m_EntityManager = nullptr;
	//std::mutex m_RegionMutex;

	MapSystem()
	{
		AddComponentSignature<Map>();
		m_EntityManager = &EntityManager::Get();

		HotReloader::Get()->SubscribeToFileChange("Assets\\Data\\Regions.json", std::bind(&MapSystem::RegionsChanged, this, std::placeholders::_1, std::placeholders::_2));
		HotReloader::Get()->SubscribeToFileChange("Assets\\Map\\RegionMap.txt", std::bind(&MapSystem::RegionsChanged, this, std::placeholders::_1, std::placeholders::_2));

		HotReloader::Get()->SubscribeToFileChange("Assets\\Shaders\\LandShader.frag", std::bind(&MapSystem::ShadersChanged, this, std::placeholders::_1, std::placeholders::_2));
		HotReloader::Get()->SubscribeToFileChange("Assets\\Shaders\\LandShader.vert", std::bind(&MapSystem::ShadersChanged, this, std::placeholders::_1, std::placeholders::_2));
	}
	
	virtual void Start() override
	{
		Map* maps = m_EntityManager->GetComponentArray<Map>();
		sf::Vector2 resolution = Window::GetWindow()->getSize();
		for (auto& entity : m_Entities)
		{
			unsigned int regionIndex = 0;
			for (auto& region : maps[entity].m_Regions)
			{
				UpdateMapInfo(maps, entity, regionIndex);
				regionIndex++;
			}
		}
	}

	virtual void Update() override
	{
		Map* maps = m_EntityManager->GetComponentArray<Map>();
		sf::Vector2 resolution = Window::GetWindow()->getSize();
		for (auto& entity : m_Entities)
		{
			if (maps[entity].m_UpdateMapInfo == true)
			{
				unsigned int regionIndex = 0;
				for (auto& region : maps[entity].m_Regions)
				{
					maps[entity].m_UpdateMapInfo = false;
					UpdateMapInfo(maps, entity, regionIndex);
					regionIndex++;
				}
			}
		}
	}

	virtual void Render() override
	{
		Map* maps = m_EntityManager->GetComponentArray<Map>();
		for (auto& entity : m_Entities)
		{
			for (auto& region : maps[entity].m_Regions)
			{
				maps[entity].m_LandShader.setUniform("u_Color", sf::Glsl::Vec4(region.m_HighlightColor));
				Window::GetWindow()->draw(region.m_VertexArray, maps[entity].m_RenderStates);
			}
		}
	}

	bool IsBorderTile(const std::vector<Vector2DInt>& tiles, const Vector2DInt currentTile)
	{
		// Figure out if the tile only borders same tile as this region or not

		sf::Vector2f upLeft(currentTile.x - 1.0f, currentTile.y + 1.0f);
		sf::Vector2f up(currentTile.x, currentTile.y + 1.0f);
		sf::Vector2f upRight(currentTile.x + 1.0f, currentTile.y + 1.0f);
		sf::Vector2f left(currentTile.x - 1.0f, currentTile.y);
		sf::Vector2f right(currentTile.x + 1.0f, currentTile.y);
		sf::Vector2f downRight(currentTile.x + 1.0f, currentTile.y - 1.0f);
		sf::Vector2f down(currentTile.x, currentTile.y -1.0f);
		sf::Vector2f downLeft(currentTile.x -1.0f, currentTile.y -1.0f);

		for (auto& t : tiles)
		{
			float currentX = (float)t.x;
			float currentY = (float)t.y;

			if (upLeft.x == currentX && upLeft.y == currentY)
			{
				return true;
			}

			if (up.x == currentX && up.y == currentY)
			{
				return true;
			}

			if (upRight.x == currentX && upRight.y == currentY)
			{
				return true;
			}

			if (right.x == currentX && right.y == currentY)
			{
				return true;
			}

			if (downRight.x == currentX && downRight.y == currentY)
			{
				return true;
			}

			if (down.x == currentX && down.y == currentY)
			{
				return true;
			}

			if (downLeft.x == currentX && downLeft.y == currentY)
			{
				return true;
			}
		}

		return false;
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
			//mtx->lock();
			maps[entity].LoadAllRegions();
			maps[entity].LoadMap();
			maps[entity].CreateVertexArrays();
			//mtx->unlock();
		}
	}

	void ShadersChanged(std::string path, FileStatus fileStatus)
	{
		if (fileStatus != FileStatus::Modified)
		{
			return;
		}

		Map* maps = m_EntityManager->GetComponentArray<Map>();
		for (auto& entity : m_Entities)
		{
			//mtx->lock();
			maps[entity].LoadShadersAndCreateRenderStates();
			//mtx->unlock();
		}
	}
	
	void UpdateMapInfo(Map* maps, sf::Uint32 entity, unsigned int regionIndex)
	{
		MapInfo::SetRegionName(maps[entity].m_Regions[regionIndex].m_RegionName, regionIndex);
		MapInfo::SetRegionTax(maps[entity].m_Regions[regionIndex].m_RegionTax, regionIndex);
		MapInfo::SetOwnerName(std::to_string(maps[entity].m_Regions[regionIndex].m_RegionId), regionIndex);
		MapInfo::SetMapRegions(maps[entity].m_Regions);
	}
};