#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Engine/Time.h"
#include "Engine/Window.h"
#include "ECS/Components/Transform.h"
#include "Game/Components/UIText.h"
#define _USE_MATH_DEFINES
#include <math.h>


struct UITextSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	sf::RenderWindow* m_Window;

	// Constructor, Runs when the system is initialized
	// Do any kind of init here but remember to register
	// the systems component signature. IE: add every component
	// that the system needs or should look for
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
			AdjustText(&UITexts[entity]);
			UITexts[entity].m_CountryNameText.setFont(UITexts[entity].m_Font);
			UITexts[entity].m_CountryNameText.setCharacterSize(UITexts[entity].m_CharacterSize);
			UITexts[entity].m_CountryNameText.setStyle(UITexts[entity].m_Style);
			UITexts[entity].m_CountryNameText.setString(UITexts[entity].m_CountryName);
			UITexts[entity].m_CountryNameText.setPosition(UITexts[entity].m_PositionX, UITexts[entity].m_PositionY);
			UITexts[entity].m_CountryNameText.setFillColor(UITexts[entity].m_FillColor);
			UITexts[entity].m_CountryNameText.setOutlineColor(UITexts[entity].m_OutlineColor);
			UITexts[entity].m_CountryNameText.setOutlineThickness(UITexts[entity].m_OutlineThickness);
			UITexts[entity].m_CountryNameText.setRotation(UITexts[entity].m_Rotation);
		}
	}

	// Update gets called every frame and loops through every entity that has the signature that
	// the system has registered and do the necessary update
	virtual void Update() override
	{
		// Transform* transforms = m_EntityManager->GetComponentArray<Transform>(); // Uncommented because it's unused
		UIText* UITexts = m_EntityManager->GetComponentArray<UIText>();

		for (auto entity : m_Entities)
		{
			if (UITexts[entity].m_AdjustText == true)
			{
				UITexts[entity].m_AdjustText = false;
				AdjustText(&UITexts[entity]);
				UITexts[entity].m_CountryNameText.setFont(UITexts[entity].m_Font);
				UITexts[entity].m_CountryNameText.setCharacterSize(UITexts[entity].m_CharacterSize);
				UITexts[entity].m_CountryNameText.setStyle(UITexts[entity].m_Style);
				UITexts[entity].m_CountryNameText.setString(UITexts[entity].m_CountryName);
				UITexts[entity].m_CountryNameText.setPosition(UITexts[entity].m_PositionX, UITexts[entity].m_PositionY);
				UITexts[entity].m_CountryNameText.setFillColor(UITexts[entity].m_FillColor);
				UITexts[entity].m_CountryNameText.setOutlineColor(UITexts[entity].m_OutlineColor);
				UITexts[entity].m_CountryNameText.setOutlineThickness(UITexts[entity].m_OutlineThickness);
				UITexts[entity].m_CountryNameText.setRotation(UITexts[entity].m_Rotation);
			}
		}
	}

	virtual void Render() override
	{
		UIText* UITexts = m_EntityManager->GetComponentArray<UIText>();

		for (auto entity : m_Entities)
		{
			m_Window->draw(UITexts[entity].m_CountryNameText);
		}
	}

	void AdjustText(UIText* UIText)
	{
		std::vector<MapRegion> regions = MapInfo::GetMapRegions();
		Vector2DInt leftMostPosition = {100, 0};
		Vector2DInt rightMostPosition = {0, 0};
		unsigned int regionIndex = 0;
		for each (MapRegion region in regions)
		{
			for each (unsigned int index in UIText->m_OwnedRegions)
			{
				if (index == regionIndex)
				{
					for each (Vector2DInt position in region.m_MapSquares)
					{
						if (position.x <= leftMostPosition.x)
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
		Vector2D leftMostPositionScreen = MapInfo::ConvertToScreen(leftMostPosition);
		Vector2D diagonal = MapInfo::ConvertToScreen(rightMostPosition) - leftMostPositionScreen;
		UIText->m_CharacterSize = (unsigned int)(diagonal.x * 0.1f);
		UIText->m_PositionX = diagonal.x * 0.5f + leftMostPositionScreen.x - UIText->m_CharacterSize * 3;
		UIText->m_PositionY = diagonal.y * 0.5f + leftMostPositionScreen.y - UIText->m_CharacterSize;
		UIText->m_Rotation = (float)(std::atan2f(diagonal.y, diagonal.x) * 180.0f) / (float)M_PI;
	}
};