#pragma once

#include <SFML/Graphics.hpp>
#include "ECS/Component.h"

const static unsigned int NUMBER_OF_BUILDING_SLOTS = 4;

struct RegionWindow : public Component
{
	sf::RectangleShape m_WindowShape;
	sf::RectangleShape m_BuildingSlotShapes[NUMBER_OF_BUILDING_SLOTS];
	sf::RectangleShape m_RaiseArmyShape;
	sf::Color m_BuildingSlotColors[NUMBER_OF_BUILDING_SLOTS];
	sf::Color m_FillColor = sf::Color(255, 252, 240);
	sf::Color m_OwnerColor = sf::Color::Red;
	sf::Color m_RaiseArmyColor = sf::Color::Transparent;
	float m_SizeX = 600.0f;
	float m_SizeY = 600.0f;
	float m_OutlineThickness = 10.0f;
	sf::Text m_RegionNameText;
	sf::Text m_RegionTaxText;
	sf::Text m_KingdomNameText;
	sf::Font m_Font;
	std::string m_RegionName = "";
	std::string m_RegionTax = "";
	std::string m_KingdomName = "";
	int m_CharacterSize = 50;
	unsigned int m_RegionPortraitID = 0;
	EntityID m_BuildingIconIDs[NUMBER_OF_BUILDING_SLOTS];
	unsigned int m_CharacterWindowID = 0;
	unsigned int m_BottomPortraitID = 0;
	unsigned int m_RaiseArmyID = 0;
	bool m_Visible = false;
	bool m_Open = false;
	unsigned int m_CurrentRegionID = 0;
	unsigned int m_NumberOfBuildingSlots = NUMBER_OF_BUILDING_SLOTS;
	sf::RectangleShape m_BuildingProgressShape[NUMBER_OF_BUILDING_SLOTS];

#pragma warning(push)
#pragma warning(disable: 26812)
	sf::Text::Style m_Style = sf::Text::Regular;
#pragma warning(pop)

	RegionWindow()
	{
		for (unsigned int index = 0; index < m_NumberOfBuildingSlots; index++)
		{
			m_BuildingIconIDs[index] = index;
		}
	}

	RegionWindow(sf::Font font, unsigned int regionPortraitID, EntityID regionBuildSlotID[NUMBER_OF_BUILDING_SLOTS], unsigned int raiseArmyID, unsigned int characterWindowID, unsigned int bottomPortraitID)
	{
		m_Font = font;
		m_RegionPortraitID = regionPortraitID;
		for (unsigned int index = 0; index < m_NumberOfBuildingSlots; index++)
		{
			m_BuildingIconIDs[index] = regionBuildSlotID[index];
		}
		m_RaiseArmyID = raiseArmyID;
		m_CharacterWindowID = characterWindowID;
		m_BottomPortraitID = bottomPortraitID;
	}
};