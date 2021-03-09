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
	}

	virtual void Update() override
	{
		Map* maps = m_EntityManager->GetComponentArray<Map>();
		sf::Vector2 resolution = Window::GetWindow()->getSize();
		for (auto& entity : m_Entities)
		{
			unsigned int regionIndex = 0;
			for (auto& region : maps[entity].m_Regions)
			{
				MapInfo::SetRegionName(maps[entity].m_Regions[regionIndex].m_RegionName, regionIndex);
				MapInfo::SetRegionTax(maps[entity].m_Regions[regionIndex].m_RegionTax, regionIndex);
				MapInfo::SetOwnerName(std::to_string(maps[entity].m_Regions[regionIndex].m_RegionId), regionIndex);
				unsigned int squareIndex = 0;
				for (auto& square : region.m_MapSquares)
				{
					float spritePositionX = resolution.x * 0.1f + (square.x + maps[entity].m_XOffset) * 32 * maps[entity].m_MapScale;
					float spritePositionY = (square.y + maps[entity].m_YOffset) * 32 * maps[entity].m_MapScale;
					maps[entity].m_Regions[regionIndex].m_RegionPositions[squareIndex] = Vector2DInt(spritePositionX, spritePositionY);
					squareIndex++;
				}
				MapInfo::SetRegionPositions(maps[entity].m_Regions[regionIndex].m_RegionPositions, regionIndex);
				regionIndex++;
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
				//const sf::Vector2f shapeSize{ 30.0f, 30.0f };
				//sf::VertexArray shape;
				//shape.setPrimitiveType(sf::Quads); // Maybe quads??
				//maps[entity].m_LandTexture.setRepeated(true);

				for (auto& square : region.m_MapSquares)
				{
					//sf::Vector2f position;
					//sf::Vector2f vertexPosition; // shape vertex positions ranging from 0 - shape size (position scaled to shape size)
					//sf::Vector2f texturePosition; // shape texture positions ranging from 0 - texture size (texture scaled to texture size)

					//position = { (float)square.x, (float)square.y };
					//vertexPosition = { position.x * shapeSize.x, position.y * shapeSize.y };
					//texturePosition = { 0.0f, 1.0f };

					//sf::Vector2f internalPositions[] = {
					//	{ -0.5f, -0.5f, },
					//	{ 0.05f, -0.5f },
					//	{ 0.5f, 0.5f },
					//	{ -0.5f, 0.5f }
					//};

					//for (int i = 0; i < 4; ++i)
					//{
					//	sf::Vertex vertex;
					//	vertex.position = vertexPosition + internalPositions[i];
					//	vertex.texCoords = texturePosition;
					//	shape.append(vertex);
					//}


					sf::Vector2 resolution = Window::GetWindow()->getSize();
					maps[entity].m_LandSprite.setTexture(maps[entity].m_LandTexture);
					maps[entity].m_LandSprite.setTextureRect({ 0, 0, 32, 32 });
					maps[entity].m_LandSprite.setColor(region.m_HighlightColor);
					float spriteWidth = maps[entity].m_LandSprite.getLocalBounds().width;
					float spriteHeight = maps[entity].m_LandSprite.getLocalBounds().height;
					maps[entity].m_LandSprite.setPosition(
						resolution.x * 0.1f + (square.x + maps[entity].m_XOffset) * spriteWidth * maps[entity].m_MapScale,
						(square.y + maps[entity].m_YOffset) * spriteHeight * maps[entity].m_MapScale);
					Window::GetWindow()->draw(maps[entity].m_LandSprite);
				}
				// Window::GetWindow()->draw(shape, &maps[entity].m_LandTexture); // draw the vertex array using our texture
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
			//mtx->lock();
			maps[entity].LoadAllRegions();
			maps[entity].LoadMap();
			//mtx->unlock();
		}
	}
};