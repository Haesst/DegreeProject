#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Engine/Time.h"
#include "Engine/Window.h"
#include "ECS/Components/Transform.h"
#include "Game/Components/UIText.h";
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

	// Update gets called every frame and loops through every entity that has the signature that
	// the system has registered and do the necessary update
	virtual void Update() override
	{
		Transform* transforms = m_EntityManager->GetComponentArray<Transform>();
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
		std::vector<Vector2DInt> firstRegion = MapInfo::GetRegionPositions(UIText->m_OwnedRegions.front());
		std::vector<Vector2DInt> lastRegion = MapInfo::GetRegionPositions(UIText->m_OwnedRegions.back());
		Vector2DInt firstPosition = firstRegion.front();
		Vector2DInt lastPosition = lastRegion.back();
		std::vector<std::vector<Vector2DInt> > regions = MapInfo::GetRegions();
		unsigned int regionIndex = 0;
		for each (std::vector<Vector2DInt> region in regions)
		{
			for each (unsigned int index in UIText->m_OwnedRegions)
			{
				if (index == regionIndex)
				{
					for each (Vector2DInt position in region)
					{
						if (position.x < firstPosition.x)
						{
							firstPosition.x = position.x;
						}
						if (position.x > lastPosition.x)
						{
							lastPosition.x = position.x;
						}
					}
				}
			}
			regionIndex++;
		}
		Vector2DInt middlePosition = (firstPosition + lastPosition) / 2;
		Vector2DInt diagonal = lastPosition - firstPosition;
		UIText->m_CharacterSize = (unsigned int)(diagonal.y + diagonal.x) * 0.1f;
		UIText->m_PositionX = middlePosition.x - UIText->m_CharacterSize * 2;
		UIText->m_PositionY = middlePosition.y - UIText->m_CharacterSize;
		UIText->m_Rotation = std::atan2f((float)diagonal.y, (float)diagonal.x) * 180.0f / (float)M_PI;
	}
};