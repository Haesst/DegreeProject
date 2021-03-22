#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Engine/Time.h"
#include "Engine/Window.h"
#include "ECS/Components/Transform.h"
#include "Game/Components/UIText.h"
#define _USE_MATH_DEFINES
#include <math.h>

#pragma warning(push)
#pragma warning(disable: 26812)
struct UITextSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	sf::RenderWindow* m_Window;

	UITextSystem()
	{
		AddComponentSignature<UIText>();
		m_EntityManager = &EntityManager::Get();
		m_Window = Window::GetWindow();
	}

	virtual void Start() override
	{
		UIText* UITexts = m_EntityManager->GetComponentArray<UIText>();

		for (auto entity : m_Entities)
		{
			AdjustText(UITexts[entity]);
		}
	}

	virtual void Update() override
	{
		//UIText* UITexts = m_EntityManager->GetComponentArray<UIText>();

		//for (auto entity : m_Entities)
		//{

		//}
	}

	virtual void Render() override
	{
		UIText* UITexts = m_EntityManager->GetComponentArray<UIText>();

		for (auto entity : m_Entities)
		{
			if (!UITexts[entity].m_Conquered)
			{
				m_Window->draw(UITexts[entity].m_CountryNameText);
			}
		}
	}

	void AdjustText(UIText& UIText)
	{
		if (UIText.m_OwnedRegionIDs.size() > 0)
		{
			std::vector<MapRegion> regions = Map::m_Data.m_Regions;
			Vector2DInt leftMostPosition = { 100, 0 };
			Vector2DInt rightMostPosition = { 0, 0 };
			unsigned int regionIndex = 0;
			for each (MapRegion region in regions)
			{
				for each (unsigned int regionId in UIText.m_OwnedRegionIDs)
				{
					if (regionId == regions[regionIndex].m_RegionId)
					{
						for each (Vector2DInt position in region.m_MapSquares)
						{
							if (position.x < leftMostPosition.x)
							{
								leftMostPosition = position;
							}
							if (position.x >= rightMostPosition.x)
							{
								rightMostPosition = position;
							}
						}
					}
				}
				regionIndex++;
			}
			Vector2D leftMostPositionScreen = Map::ConvertToScreen(leftMostPosition);
			Vector2D diagonal = Map::ConvertToScreen(rightMostPosition) - leftMostPositionScreen;
			float offsetY = 1.0f;
			UIText.m_CharacterSize = (unsigned int)(diagonal.x * 0.1f);
			UIText.m_Rotation = (float)(std::atan2f(diagonal.y, diagonal.x) * 180.0f) / (float)M_PI;
			if (UIText.m_Rotation < 0.0f)
			{
				offsetY = -0.5f;
			}
			else if (UIText.m_Rotation > 0.0f)
			{
				offsetY = 1.5f;
			}
			UIText.m_PositionX = diagonal.x * 0.5f + leftMostPositionScreen.x - UIText.m_CharacterSize * 3;
			UIText.m_PositionY = diagonal.y * 0.5f + leftMostPositionScreen.y - UIText.m_CharacterSize * offsetY;
			UIText.m_CountryNameText.setFont(UIText.m_Font);
			UIText.m_CountryNameText.setCharacterSize(UIText.m_CharacterSize);
			UIText.m_CountryNameText.setStyle(UIText.m_Style);
			UIText.m_CountryNameText.setString(UIText.m_CountryName);
			UIText.m_CountryNameText.setPosition(UIText.m_PositionX, UIText.m_PositionY);
			UIText.m_CountryNameText.setFillColor(UIText.m_FillColor);
			UIText.m_CountryNameText.setOutlineColor(UIText.m_OutlineColor);
			UIText.m_CountryNameText.setOutlineThickness(UIText.m_OutlineThickness);
			UIText.m_CountryNameText.setRotation(UIText.m_Rotation);
		}
		else
		{
			UIText.m_Conquered = true;
		}
	}

	void ConquerRegion(unsigned int regionID, unsigned conqueringEntity)
	{
		UIText& text = m_EntityManager->GetComponent<UIText>(conqueringEntity + 1);
		text.m_OwnedRegionIDs.push_back(regionID);
		AdjustText(text);
	}

	void LoseRegion(unsigned regionIndex, unsigned int losingEntity)
	{
		UIText& text = m_EntityManager->GetComponent<UIText>(losingEntity + 1);
		text.m_OwnedRegionIDs.erase(text.m_OwnedRegionIDs.begin() + regionIndex);
		AdjustText(text);
	}
};

#pragma warning(pop)