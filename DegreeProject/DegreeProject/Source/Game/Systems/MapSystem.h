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
	std::mutex m_RegionMutex;

	MapSystem()
	{
		AddComponentSignature<OldMap>();
		m_EntityManager = &EntityManager::Get();

		HotReloader::Get()->SubscribeToFileChange("Assets\\Data\\Regions.json", std::bind(&MapSystem::RegionsChanged, this, std::placeholders::_1, std::placeholders::_2));
		HotReloader::Get()->SubscribeToFileChange("Assets\\Map\\RegionMap.txt", std::bind(&MapSystem::RegionsChanged, this, std::placeholders::_1, std::placeholders::_2));

		HotReloader::Get()->SubscribeToFileChange("Assets\\Shaders\\LandShader.frag", std::bind(&MapSystem::ShadersChanged, this, std::placeholders::_1, std::placeholders::_2));
		HotReloader::Get()->SubscribeToFileChange("Assets\\Shaders\\LandShader.vert", std::bind(&MapSystem::ShadersChanged, this, std::placeholders::_1, std::placeholders::_2));
	}
	
	virtual void Start() override
	{
		OldMap* maps = m_EntityManager->GetComponentArray<OldMap>();
		for (auto& entity : m_Entities)
		{
			//UpdateMapInfo(maps, entity);
			for (size_t i = 0; i < maps[entity].m_Regions.size(); ++i)
			{
				//UpdateMapInfo(maps, entity, i);
			}
		}
	}

	virtual void Update() override
	{
		OldMap* maps = m_EntityManager->GetComponentArray<OldMap>();
		for (auto& entity : m_Entities)
		{
			if (maps[entity].m_UpdateMapInfo == true)
			{
				//UpdateMapInfo(maps, entity);
				maps[entity].m_UpdateMapInfo = false;
				for (size_t i = 0; i < maps[entity].m_Regions.size(); ++i)
				{
					//UpdateMapInfo(maps, entity, i);
				}
			}
		}
	}

	virtual void Render() override
	{
		OldMap* maps = m_EntityManager->GetComponentArray<OldMap>();
		for (auto& entity : m_Entities)
		{
			for (auto& region : maps[entity].m_Regions)
			{
				maps[entity].m_LandShader.setUniform("u_Color", sf::Glsl::Vec4(region.m_HighlightColor));
				maps[entity].m_LandShader.setUniform("u_Texture", maps[entity].m_LandTexture);
				Window::GetWindow()->draw(region.m_VertexArray, maps[entity].m_RenderStates);
			}
		}
	}

	void RegionsChanged(std::string path, FileStatus fileStatus)
	{
		if (fileStatus != FileStatus::Modified)
		{
			return;
		}

		OldMap* maps = m_EntityManager->GetComponentArray<OldMap>();
		for (auto& entity : m_Entities)
		{
			m_RegionMutex.lock();
			maps[entity].ClearRegions();
			maps[entity].LoadAllRegions();
			maps[entity].ClearRegionMapTiles();
			maps[entity].LoadMap();
			maps[entity].CreateVertexArrays();
			m_RegionMutex.unlock();
		}
	}

	void ShadersChanged(std::string path, FileStatus fileStatus)
	{
		if (fileStatus != FileStatus::Modified)
		{
			return;
		}

		OldMap* maps = m_EntityManager->GetComponentArray<OldMap>();
		for (auto& entity : m_Entities)
		{
			//mtx->lock();
			maps[entity].LoadShadersAndCreateRenderStates();
			//mtx->unlock();
		}
	}
	
	void UpdateMapInfo(OldMap* maps, sf::Uint32 entity)
	{
		//MapInfo::SetMapOffset(maps[entity].m_XOffset, maps[entity].m_YOffset);
		//MapInfo::SetMapRegions(maps[entity].m_Regions);
	}

	void UpdateRegionMapInfo(OldMap* maps, sf::Uint32 entity, size_t regionIndex)
	{
		//MapInfo::SetRegionName(maps[entity].m_Regions[regionIndex].m_RegionName, (unsigned int)regionIndex);
		//MapInfo::SetRegionTax(maps[entity].m_Regions[regionIndex].m_RegionTax, (unsigned int)regionIndex);
		//MapInfo::SetRegionCapital(maps[entity].m_Regions[regionIndex].m_RegionCapital, (unsigned int)regionIndex);
	}
};
