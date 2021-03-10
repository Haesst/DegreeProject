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

	virtual void Start() override
	{
		Map* maps = m_EntityManager->GetComponentArray<Map>();
		sf::Vector2 resolution = Window::GetWindow()->getSize();
		for (auto& entity : m_Entities)
		{
			unsigned int regionIndex = 0;
			for (auto& region : maps[entity].m_Regions)
			{
				maps[entity].m_UpdateMapInfo = false;
				UpdateMapInfo(maps, entity, regionIndex, region, resolution);
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
			unsigned int regionIndex = 0;
			for (auto& region : maps[entity].m_Regions)
			{
				if (maps[entity].m_UpdateMapInfo == true)
				{
					maps[entity].m_UpdateMapInfo = false;
					UpdateMapInfo(maps, entity, regionIndex, region, resolution);
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
				for (auto& square : region.m_MapSquares)
				{
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
				//sf::VertexArray va;
				//va.setPrimitiveType(sf::Triangles);

				//sf::VertexArray va2;
				//va.setPrimitiveType(sf::Triangles);

				//float squareSize = 32.0f;
				//float halfSquare = squareSize * 0.5f;

				//float xOffset = 100.0f;
				//float yOffset = 100.0f;

				//int i = 0;

				//for (auto& square : region.m_MapSquares)
				//{
				//	sf::Vertex v1;
				//	sf::Vertex v2;
				//	sf::Vertex v3;
				//	sf::Vertex v4;

				//	sf::Vertex vav1;
				//	sf::Vertex vav2;
				//	sf::Vertex vav3;
				//	sf::Vertex vav4;

				//	v1.position = { (square.x * squareSize) - halfSquare + xOffset, (square.y * squareSize) - halfSquare + yOffset }; // Top left
				//	v1.color = sf::Color::Green;
				//	v2.position = { (square.x * squareSize) + halfSquare + xOffset, (square.y * squareSize) - halfSquare + yOffset }; // Top right
				//	v2.color = sf::Color::Green;
				//	v3.position = { (square.x * squareSize) + halfSquare + xOffset, (square.y * squareSize) + halfSquare + yOffset }; // Bottom right
				//	v3.color = sf::Color::Green;
				//	v4.position = { (square.x * squareSize) - halfSquare + xOffset, (square.y * squareSize) + halfSquare + yOffset }; // Bottom Left
				//	v4.color = sf::Color::Green;

				//	vav1.position = { ((square.x * squareSize) - halfSquare) + xOffset, ((square.y * squareSize) - halfSquare) * 0.8f + yOffset }; // Top left
				//	vav1.color = sf::Color::Green;
				//	vav2.position = { ((square.x * squareSize) + halfSquare) + xOffset, ((square.y * squareSize) - halfSquare) * 0.8f + yOffset }; // Top right
				//	vav2.color = sf::Color::Green;
				//	vav3.position = { ((square.x * squareSize) + halfSquare) + xOffset, ((square.y * squareSize) + halfSquare) * 0.8f + yOffset }; // Bottom right
				//	vav3.color = sf::Color::Green;
				//	vav4.position = { ((square.x * squareSize) - halfSquare) + xOffset, ((square.y * squareSize) + halfSquare) * 0.8f + yOffset }; // Bottom Left
				//	vav4.color = sf::Color::Green;

				//	va.append(v1);
				//	va.append(v2);
				//	va.append(v3);
				//	va.append(v1);
				//	va.append(v3);
				//	va.append(v4);

				//	va2.append(vav1);
				//	va2.append(vav2);
				//	va2.append(vav3);
				//	va2.append(vav1);
				//	va2.append(vav3);
				//	va2.append(vav4);
				//	i++;
				//}

				//sf::Shader borderShader;
				//borderShader.loadFromFile("Assets/Shaders/BorderShader.frag", sf::Shader::Fragment);

				//sf::RenderStates borderStates;
				//borderStates.shader = &borderShader;

				//sf::Shader landVertShader;
				//landVertShader.loadFromFile("Assets/Shaders/LandShader.vert", sf::Shader::Vertex);
				//landVertShader.loadFromFile("Assets/Shaders/LandShader.frag", sf::Shader::Fragment);

				//sf::RenderStates landStates;
				//landStates.shader = &landVertShader;

				//Window::GetWindow()->draw(va, borderStates);
				//Window::GetWindow()->draw(va2, landStates);
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
			//mtx->unlock();
		}
	}

	void UpdateMapInfo(Map* maps, sf::Uint32 entity, unsigned int regionIndex, MapRegion region, sf::Vector2<unsigned int> resolution)
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
	}
};