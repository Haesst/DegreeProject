#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Engine/Window.h"
#include "Game/Components/CharacterWindow.h"
#include "Engine/InputHandler.h"

struct CharacterWindowSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	sf::RenderWindow* m_Window = nullptr;
	CharacterWindow* m_CharacterWindows = nullptr;
	CharacterComponent* playerCharacter = nullptr;
	MapRegion* m_CurrentMapRegion = nullptr;
	bool m_PlayerRegion = false;

	CharacterWindowSystem()
	{
		AddComponentSignature<CharacterWindow>();
		m_EntityManager = &EntityManager::Get();
		m_Window = Window::GetWindow();
	}

	~CharacterWindowSystem(){}

	virtual void Start() override
	{
		m_CharacterWindows = m_EntityManager->GetComponentArray<CharacterWindow>();
#pragma warning(push)
#pragma warning(disable: 26815)
		CharacterSystem* characterSystem = (CharacterSystem*)m_EntityManager->GetSystem<CharacterSystem>().get();
#pragma warning(pop)
		playerCharacter = &m_EntityManager->GetComponent<CharacterComponent>(characterSystem->GetPlayerID());
		for (auto entity : m_Entities)
		{
			m_CharacterWindows[entity].m_Shape.setFillColor(m_CharacterWindows[entity].m_FillColor);
			m_CharacterWindows[entity].m_Shape.setOutlineThickness(m_CharacterWindows[entity].m_OutlineThickness);

			m_CharacterWindows[entity].m_DeclareWarShape.setFillColor(sf::Color::Transparent);
			m_CharacterWindows[entity].m_DeclareWarShape.setOutlineColor(m_CharacterWindows[entity].m_DeclareWarColor);
			m_CharacterWindows[entity].m_DeclareWarShape.setOutlineThickness(m_CharacterWindows[entity].m_OutlineThickness);
			m_CharacterWindows[entity].m_DeclareWarShape.setSize(sf::Vector2f(m_CharacterWindows[entity].m_SizeX * 0.2f, m_CharacterWindows[entity].m_SizeY * 0.05f));

			m_CharacterWindows[entity].m_DeclareWarText.setFont(m_CharacterWindows[entity].m_Font);
			m_CharacterWindows[entity].m_DeclareWarText.setCharacterSize((int)(m_CharacterWindows[entity].m_CharacterSize * 0.5f));
			m_CharacterWindows[entity].m_DeclareWarText.setStyle(m_CharacterWindows[entity].m_Style);
			m_CharacterWindows[entity].m_DeclareWarText.setString(m_CharacterWindows[entity].m_DeclareWar);
			m_CharacterWindows[entity].m_DeclareWarText.setFillColor(m_CharacterWindows[entity].m_DeclareWarColor);

			m_CharacterWindows[entity].m_MakePeaceShape.setFillColor(sf::Color::Transparent);
			m_CharacterWindows[entity].m_MakePeaceShape.setOutlineColor(m_CharacterWindows[entity].m_MakePeaceColor);
			m_CharacterWindows[entity].m_MakePeaceShape.setOutlineThickness(m_CharacterWindows[entity].m_OutlineThickness);
			m_CharacterWindows[entity].m_MakePeaceShape.setSize(sf::Vector2f(m_CharacterWindows[entity].m_SizeX * 0.2f, m_CharacterWindows[entity].m_SizeY * 0.05f));

			m_CharacterWindows[entity].m_MakePeaceText.setFont(m_CharacterWindows[entity].m_Font);
			m_CharacterWindows[entity].m_MakePeaceText.setCharacterSize((int)(m_CharacterWindows[entity].m_CharacterSize * 0.5f));
			m_CharacterWindows[entity].m_MakePeaceText.setStyle(m_CharacterWindows[entity].m_Style);
			m_CharacterWindows[entity].m_MakePeaceText.setString(m_CharacterWindows[entity].m_MakePeace);
			m_CharacterWindows[entity].m_MakePeaceText.setFillColor(m_CharacterWindows[entity].m_MakePeaceColor);

			m_CharacterWindows[entity].m_OwnerCharacterText.setFont(m_CharacterWindows[entity].m_Font);
			m_CharacterWindows[entity].m_OwnerCharacterText.setCharacterSize(m_CharacterWindows[entity].m_CharacterSize);
			m_CharacterWindows[entity].m_OwnerCharacterText.setStyle(m_CharacterWindows[entity].m_Style);

			m_CharacterWindows[entity].m_OwnerNameText.setFont(m_CharacterWindows[entity].m_Font);
			m_CharacterWindows[entity].m_OwnerNameText.setCharacterSize(m_CharacterWindows[entity].m_CharacterSize);
			m_CharacterWindows[entity].m_OwnerNameText.setStyle(m_CharacterWindows[entity].m_Style);

			m_CharacterWindows[entity].m_RegionNameText.setFont(m_CharacterWindows[entity].m_Font);
			m_CharacterWindows[entity].m_RegionNameText.setCharacterSize(m_CharacterWindows[entity].m_CharacterSize);
			m_CharacterWindows[entity].m_RegionNameText.setStyle(m_CharacterWindows[entity].m_Style);

			m_CharacterWindows[entity].m_TaxText.setFont(m_CharacterWindows[entity].m_Font);
			m_CharacterWindows[entity].m_TaxText.setCharacterSize(m_CharacterWindows[entity].m_CharacterSize);
			m_CharacterWindows[entity].m_TaxText.setStyle(m_CharacterWindows[entity].m_Style);
		}
	}

	virtual void Update() override
	{
		for (auto entity : m_Entities)
		{
			UpdateInfo(m_CharacterWindows[entity]);
			HandleWindow(m_CharacterWindows[entity]);

			if (m_CharacterWindows[entity].m_Visible)
			{
				ClickButton(m_CharacterWindows[entity]);

				m_CharacterWindows[entity].m_Shape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)m_CharacterWindows[entity].m_OutlineThickness, (int)m_CharacterWindows[entity].m_OutlineThickness)));
				m_CharacterWindows[entity].m_Shape.setOutlineColor(m_CharacterWindows[entity].m_OwnerColor);

				m_CharacterWindows[entity].m_DeclareWarShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_CharacterWindows[entity].m_SizeX * 0.775f), (int)(m_CharacterWindows[entity].m_SizeY * 0.032f))));
				m_CharacterWindows[entity].m_DeclareWarText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_CharacterWindows[entity].m_SizeX * 0.785f), (int)(m_CharacterWindows[entity].m_SizeY * 0.04f))));

				m_CharacterWindows[entity].m_MakePeaceShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_CharacterWindows[entity].m_SizeX * 0.775f), (int)(m_CharacterWindows[entity].m_SizeY * 0.112f))));
				m_CharacterWindows[entity].m_MakePeaceText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_CharacterWindows[entity].m_SizeX * 0.785f), (int)(m_CharacterWindows[entity].m_SizeY * 0.12f))));

				m_CharacterWindows[entity].m_OwnerCharacterText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_CharacterWindows[entity].m_SizeX * 0.25f), (int)(m_CharacterWindows[entity].m_SizeY * 0.025f))));
				m_CharacterWindows[entity].m_OwnerCharacterText.setString(m_CharacterWindows[entity].m_OwnerTitle + m_CharacterWindows[entity].m_OwnerCharacterName);
				m_CharacterWindows[entity].m_OwnerCharacterText.setFillColor(m_CharacterWindows[entity].m_OwnerColor);

				m_CharacterWindows[entity].m_OwnerNameText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_CharacterWindows[entity].m_SizeX * 0.1f), (int)(m_CharacterWindows[entity].m_SizeY * 0.1f))));
				m_CharacterWindows[entity].m_OwnerNameText.setString(m_CharacterWindows[entity].m_OwnerKingdomName);
				m_CharacterWindows[entity].m_OwnerNameText.setFillColor(m_CharacterWindows[entity].m_OwnerColor);

				m_CharacterWindows[entity].m_RegionNameText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_CharacterWindows[entity].m_SizeX * 0.1f), (int)(m_CharacterWindows[entity].m_SizeY * 0.2f))));
				m_CharacterWindows[entity].m_RegionNameText.setString("Region: " + m_CharacterWindows[entity].m_RegionName);
				m_CharacterWindows[entity].m_RegionNameText.setFillColor(m_CharacterWindows[entity].m_OwnerColor);

				m_CharacterWindows[entity].m_TaxText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_CharacterWindows[entity].m_SizeX * 0.1f), (int)(m_CharacterWindows[entity].m_SizeY * 0.3f))));
				m_CharacterWindows[entity].m_TaxText.setString("Tax: " + std::to_string(m_CharacterWindows[entity].m_RegionTax));
				m_CharacterWindows[entity].m_TaxText.setFillColor(m_CharacterWindows[entity].m_OwnerColor);
			}
		}
	}

	virtual void Render() override
	{
		for (auto entity : m_Entities)
		{
			if (m_CharacterWindows[entity].m_Visible)
			{
				m_Window->draw(m_CharacterWindows[entity].m_Shape);
				m_Window->draw(m_CharacterWindows[entity].m_OwnerNameText);
				m_Window->draw(m_CharacterWindows[entity].m_RegionNameText);
				m_Window->draw(m_CharacterWindows[entity].m_TaxText);
				m_Window->draw(m_CharacterWindows[entity].m_OwnerCharacterText);
				if (!m_PlayerRegion)
				{
					m_Window->draw(m_CharacterWindows[entity].m_DeclareWarShape);
					m_Window->draw(m_CharacterWindows[entity].m_DeclareWarText);
					m_Window->draw(m_CharacterWindows[entity].m_MakePeaceShape);
					m_Window->draw(m_CharacterWindows[entity].m_MakePeaceText);
				}
			}
		}
	}

	void UpdateInfo(CharacterWindow& characterWindow)
	{
		if (InputHandler::GetRightMouseClicked() && !InputHandler::GetPlayerUnitSelected())
		{
			Vector2D mousePosition = InputHandler::GetMousePosition();
			if (!characterWindow.m_Shape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				Vector2DInt mouseMapPosition = InputHandler::GetMouseMapPosition();
				if (Map::MapSquareDataContainsKey(mouseMapPosition))
				{
					EntityID regionID = 0;
					for (auto& squareData : Map::m_MapSquareData)
					{
						if (squareData.m_Position == mouseMapPosition)
						{
							regionID = squareData.m_RegionID;
						}
					}

					if (characterWindow.m_CurrentRegionID != regionID)
					{
						characterWindow.m_CurrentRegionID = regionID;
						CheckIfPlayerRegion(characterWindow.m_CurrentRegionID);
						m_CurrentMapRegion = &Map::GetRegionById(characterWindow.m_CurrentRegionID);
						characterWindow.m_RegionTax = m_CurrentMapRegion->m_RegionTax;
						characterWindow.m_RegionName = m_CurrentMapRegion->m_RegionName;
						CharacterComponent& character = m_EntityManager->GetComponent<CharacterComponent>(m_CurrentMapRegion->m_OwnerID);
						characterWindow.m_OwnerKingdomName = character.m_KingdomName;
						characterWindow.m_OwnerCharacterName = character.m_Name;
						characterWindow.m_OwnerTitle = characterWindow.titles[(unsigned int)character.m_CharacterTitle];
						characterWindow.m_OwnerColor = character.m_RegionColor;
					}
					characterWindow.m_Open = true;
				}
				else
				{
					characterWindow.m_Open = false;
				}
			}
			else
			{
				characterWindow.m_Open = false;
			}
		}
	}

	void HandleWindow(CharacterWindow& characterWindow)
	{
		InputHandler::SetCharacterWindowOpen(characterWindow.m_Visible);
		if (InputHandler::GetRightMouseReleased() && !InputHandler::GetPlayerUnitSelected() && characterWindow.m_Open && !characterWindow.m_Visible)
		{
			OpenWindow(characterWindow);
		}
		else if (characterWindow.m_Visible && (InputHandler::GetPlayerUnitSelected() || InputHandler::GetEscapePressed()))
		{
			CloseWindow(characterWindow);
			m_EntityManager->SetEntityActive(characterWindow.m_BottomPortraitID, true);
		}
		else if (InputHandler::GetRegionWindowOpen())
		{
			CloseWindow(characterWindow);
		}
	}

	void OpenWindow(CharacterWindow& characterWindow)
	{
		characterWindow.m_Visible = true;
		characterWindow.m_Shape.setSize(sf::Vector2f(characterWindow.m_SizeX, characterWindow.m_SizeY));
		m_EntityManager->SetEntityActive(characterWindow.m_TopPortraitID, true);
		m_EntityManager->SetEntityActive(characterWindow.m_BottomPortraitID, false);
	}

	void CloseWindow(CharacterWindow& characterWindow)
	{
		characterWindow.m_Open = false;
		characterWindow.m_Visible = false;
		characterWindow.m_Shape.setSize(sf::Vector2f());
		m_EntityManager->SetEntityActive(characterWindow.m_TopPortraitID, false);
	}

	void ClickButton(CharacterWindow& characterWindow)
	{
		if (InputHandler::GetLeftMouseReleased() && !m_PlayerRegion)
		{
			Vector2D mousePosition = InputHandler::GetMousePosition();
			if (characterWindow.m_DeclareWarShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				EntityID warTarget = m_CurrentMapRegion->m_OwnerID;
				playerCharacter->DeclareWar(warTarget, characterWindow.m_CurrentRegionID);
			}
			else if (characterWindow.m_MakePeaceShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				playerCharacter->MakePeace();
			}
		}
	}

	bool CheckIfPlayerRegion(EntityID currentRegionID)
	{
		for (unsigned int ownedRegionID : playerCharacter->m_OwnedRegionIDs)
		{
			if (currentRegionID == ownedRegionID)
			{
				m_PlayerRegion = true;
				break;
			}
			else
			{
				m_PlayerRegion = false;
			}
		}
		return m_PlayerRegion;
	}
};