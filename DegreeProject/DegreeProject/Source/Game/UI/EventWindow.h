#pragma once

#include <SFML/Graphics.hpp>
#include "Engine/Window.h"
#include "Engine/InputHandler.h"
#include "Game/Systems/CharacterManager.h"
#include "Engine/AssetHandler.h"
#include "Game/Data/UIType.h"

class EventWindow
{
public:
	sf::RectangleShape m_WindowShape;

	sf::RectangleShape m_DismissShape;
	sf::Text m_DismissText;
	sf::Color m_DismissColor = sf::Color(210, 32, 60);
	sf::Vector2f m_DismissPosition = sf::Vector2f();
	const std::string m_DismissString = "Dismiss";

	sf::RectangleShape m_AgreeShape;
	sf::Text m_AgreeText;
	sf::Color m_AgreeColor = sf::Color(67, 175, 17);
	sf::Vector2f m_AgreePosition = sf::Vector2f();
	const std::string m_AgreeString = "Agree";

	sf::Color m_FillColor = sf::Color(255, 252, 240);
	sf::Color m_OwnerColor = sf::Color::Red;
	float m_SizeX = 400.0f;
	float m_SizeY = 300.0f;
	int m_PositionX = 0;
	int m_PositionY = 0;
	float m_OutlineThickness = 10.0f;
	sf::Font m_Font;
	int m_CharacterSize = 50;
	const static int m_SpriteSize = 64;
#pragma warning(push)
#pragma warning(disable: 26812)
	sf::Text::Style m_Style = sf::Text::Regular;
#pragma warning(pop)
	sf::RenderWindow* m_Window = nullptr;
	UIID m_OwnedUIWindow = INVALID_UI_ID;
	UIType m_MessageType = UIType::WarDeclaration;
	bool m_Dismissed = false;

	CharacterID m_InstigatorID = INVALID_CHARACTER_ID;
	sf::RectangleShape m_InstigatorCharacterShape;
	sf::Texture m_InstigatorTexture;
	sf::Sprite m_InstigatorSprite;
	sf::Vector2f m_InstigatorCharacterPosition = sf::Vector2f();
	sf::Text m_MessageText;
	sf::Vector2f m_MessageTextPosition = sf::Vector2f();

	Character* m_PlayerCharacter = nullptr;
	sf::RectangleShape m_PlayerCharacterShape;
	sf::Texture m_PlayerCharacterTexture;
	sf::Sprite m_PlayerCharacterSprite;
	sf::Vector2f m_PlayerCharacterPosition = sf::Vector2f();

	sf::Texture m_MessageTypeTexture;
	sf::Sprite m_MessageTypeSprite;
	sf::Vector2f m_MessageTypePosition = sf::Vector2f();

	EventWindow(UIID ID, sf::Font font, CharacterID instigatorID, UIType type)
	{
		m_OwnedUIWindow = ID;
		m_Font = font;
		m_InstigatorID = instigatorID;
		m_MessageType = type;

		m_Window = Window::getWindow();

		m_PlayerCharacter = &CharacterManager::get()->getPlayerCharacter();
		m_OwnerColor = m_PlayerCharacter->m_RegionColor;

		Character& instigator = CharacterManager::get()->getCharacter(m_InstigatorID);

		m_InstigatorTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Harriet.jpg");
		m_PlayerCharacterTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Harold.jpg");

		switch (m_MessageType)
		{
			case UIType::MarriageRequest:
			{
				m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Married.png");
				std::stringstream stream;
				stream << instigator.m_Name << " of the " << instigator.m_KingdomName << "\nwants to marry you.\n\n               Do you accept?";
				m_MessageText.setString(stream.str());
				break;
			}
			case UIType::AllianceRequest:
			{
				m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Male.png");
				std::stringstream stream;
				stream << instigator.m_Name << " of the " << instigator.m_KingdomName << "\nwants to ally you.\n\n               Do you accept?";
				m_MessageText.setString(stream.str());
				break;
			}
			case UIType::WarDeclaration:
			{
				m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/BarackIcon.png");
				std::stringstream stream;
				stream << instigator.m_Name << " of the " << instigator.m_KingdomName << "\ndeclared war on you!";
				m_MessageText.setString(stream.str());
				break;
			}
			case UIType::PeaceRequest:
			{
				m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Female.png");
				std::stringstream stream;
				stream << instigator.m_Name << " of the " << instigator.m_KingdomName << "\nwants peace.\n\n               Do you accept?";
				m_MessageText.setString(stream.str());
				break;
			}
			case UIType::Gift:
			{
				m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Coins.png");
				std::stringstream stream;
				stream << instigator.m_Name << " of the " << instigator.m_KingdomName << "\nsent you a gift of X gold";
				m_MessageText.setString(stream.str());
				break;
			}
			case UIType::Pregnant:
			{
				m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Pregnant.png");
				std::stringstream stream;
				stream << instigator.m_Name << " of the " << instigator.m_KingdomName << "\njust got pregnant!";
				m_MessageText.setString(stream.str());
				break;
			}
			case UIType::ChildBirth:
			{
				if(instigator.m_Children.size() > 0)
				{
					Character& child = CharacterManager::get()->getCharacter(instigator.m_Children.back());
					if (child.m_Gender == Gender::Male)
					{
						m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/BabyMale.png");
						std::stringstream stream;
						stream << instigator.m_Name << " of the " << instigator.m_KingdomName << "\njust gave birth to a healthy boy!";
						m_MessageText.setString(stream.str());
					}
					else
					{
						m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/BabyFemale.png");
						std::stringstream stream;
						stream << instigator.m_Name << " of the " << instigator.m_KingdomName << "\njust gave birth to a healthy girl!";
						m_MessageText.setString(stream.str());
					}
				}
				break;
			}
			case UIType::Death:
			{
				m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Dead.png");
				std::stringstream stream;
				stream << instigator.m_Name << " of the " << instigator.m_KingdomName << "\njust died!";
				m_MessageText.setString(stream.str());
				break;
			}
			default:
			{
				break;
			}	
		}

		m_PositionX = (int)(m_Window->getSize().x * 0.5f - m_OutlineThickness - m_SizeX * 0.5f);
		m_PositionY = (int)(m_Window->getSize().y * 0.5f - m_OutlineThickness - m_SizeY * 0.5f);

		m_WindowShape.setFillColor(m_FillColor);
		m_WindowShape.setOutlineThickness(m_OutlineThickness);
		m_WindowShape.setSize(sf::Vector2f(m_SizeX, m_SizeY));
		m_WindowShape.setOutlineColor(m_OwnerColor);

		m_InstigatorCharacterPosition = sf::Vector2f(m_PositionX + m_SpriteSize * 0.5f, m_PositionY + m_SpriteSize * 0.5f);
		m_InstigatorCharacterShape.setFillColor(sf::Color::Transparent);
		m_InstigatorCharacterShape.setOutlineThickness(m_OutlineThickness * 0.5f);
		m_InstigatorCharacterShape.setOutlineColor(instigator.m_RegionColor);
		m_InstigatorCharacterShape.setSize(sf::Vector2f(m_SpriteSize, m_SpriteSize));

		m_PlayerCharacterPosition = sf::Vector2f(m_PositionX + m_SizeX - m_SpriteSize * 1.5f, m_PositionY + m_SpriteSize * 0.5f);
		m_PlayerCharacterShape.setFillColor(sf::Color::Transparent);
		m_PlayerCharacterShape.setOutlineThickness(m_OutlineThickness * 0.5f);
		m_PlayerCharacterShape.setOutlineColor(m_PlayerCharacter->m_RegionColor);
		m_PlayerCharacterShape.setSize(sf::Vector2f(m_SpriteSize, m_SpriteSize));

		m_MessageTypePosition = sf::Vector2f(m_PositionX + m_SizeX * 0.5f - m_SpriteSize * 0.25f, m_PositionY + m_SpriteSize * 0.75f);

		if (m_MessageType == UIType::MarriageRequest || m_MessageType == UIType::AllianceRequest || m_MessageType == UIType::PeaceRequest)
		{
			m_AgreePosition = sf::Vector2f(m_PositionX + m_SizeX - m_SpriteSize * 1.75f, m_PositionY + m_SizeY - m_SpriteSize);
			m_AgreeShape.setSize(sf::Vector2f(m_SpriteSize * 1.25f, m_SpriteSize * 0.5f));
			m_AgreeShape.setFillColor(sf::Color::Transparent);
			m_AgreeShape.setOutlineThickness(m_OutlineThickness * 0.5f);
			m_AgreeShape.setOutlineColor(m_AgreeColor);
			m_AgreeText.setFont(m_Font);
			m_AgreeText.setCharacterSize((int)(m_CharacterSize * 0.5f));
			m_AgreeText.setStyle(m_Style);
			m_AgreeText.setFillColor(m_AgreeColor);
			m_AgreeText.setString(m_AgreeString);
		}

		m_DismissPosition = sf::Vector2f(m_PositionX + m_SpriteSize * 0.5f, m_PositionY + m_SizeY - m_SpriteSize);
		m_DismissShape.setSize(sf::Vector2f(m_SpriteSize * 1.25f, m_SpriteSize * 0.5f));
		m_DismissShape.setFillColor(sf::Color::Transparent);
		m_DismissShape.setOutlineThickness(m_OutlineThickness * 0.5f);
		m_DismissShape.setOutlineColor(m_DismissColor);
		m_DismissText.setFont(m_Font);
		m_DismissText.setCharacterSize((int)(m_CharacterSize * 0.5f));
		m_DismissText.setStyle(m_Style);
		m_DismissText.setFillColor(m_DismissColor);
		m_DismissText.setString(m_DismissString);

		m_MessageTextPosition = sf::Vector2f(m_PositionX + m_SpriteSize * 0.5f, m_PositionY + m_SpriteSize * 1.6f);
		m_MessageText.setFont(m_Font);
		m_MessageText.setCharacterSize((int)(m_CharacterSize * 0.5f));
		m_MessageText.setStyle(m_Style);
		m_MessageText.setFillColor(m_OwnerColor);
	}

	void update()
	{
		clickButton();

		m_WindowShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_PositionX, m_PositionY)));

		m_InstigatorCharacterShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_InstigatorCharacterPosition)));

		m_PlayerCharacterShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_PlayerCharacterPosition)));

		m_MessageText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_MessageTextPosition)));

		if (m_MessageType == UIType::MarriageRequest || m_MessageType == UIType::AllianceRequest || m_MessageType == UIType::PeaceRequest)
		{
			m_AgreeShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_AgreePosition)));
			m_AgreeText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_AgreePosition)));
		}

		m_DismissText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_DismissPosition)));
		m_DismissShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_DismissPosition)));
	}

	void render()
	{
		m_Window->draw(m_WindowShape);
		m_Window->draw(m_InstigatorCharacterShape);
		m_Window->draw(m_PlayerCharacterShape);
		m_Window->draw(m_MessageText);
		if (m_MessageType == UIType::MarriageRequest || m_MessageType == UIType::AllianceRequest || m_MessageType == UIType::PeaceRequest)
		{
			m_Window->draw(m_AgreeShape);
			m_Window->draw(m_AgreeText);
		}
		m_Window->draw(m_DismissShape);
		m_Window->draw(m_DismissText);
		updateSprites();
	}

	void closeWindow()
	{
		m_Dismissed = true;
	}

	void acceptRequest()
	{
		switch (m_MessageType)
		{
			case UIType::MarriageRequest:
			{
				CharacterManager::get()->marry(m_InstigatorID, m_PlayerCharacter->m_CharacterID);
				break;
			}
			case UIType::AllianceRequest:
			{
				//CharacterManager::get()->ally(m_InstigatorID, m_PlayerCharacter->m_CharacterID);
				break;
			}
			case UIType::PeaceRequest:
			{
				CharacterManager::get()->sendPeaceOffer(m_PlayerCharacter->m_CharacterID, m_InstigatorID);
				break;
			}
			default:
			{
				break;
			}
		}
	}

	void clickButton()
	{
		if (InputHandler::getLeftMouseReleased())
		{
			Vector2D mousePosition = InputHandler::getMousePosition();
			if (m_DismissShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				closeWindow();
			}
			else if (m_AgreeShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				acceptRequest();
				closeWindow();
			}
		}
		else if (InputHandler::getRightMouseReleased())
		{
			Vector2D mousePosition = InputHandler::getMousePosition();
			if (m_InstigatorCharacterShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				UIManager::get()->m_CharacterWindow->m_CurrentCharacterID = m_InstigatorID;
				UIManager::get()->m_CharacterWindow->checkIfPlayerCharacter();
				UIManager::get()->m_CharacterWindow->updateInfo();
				UIManager::get()->m_CharacterWindow->openWindow();
			}
			else if (m_PlayerCharacterShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				UIManager::get()->m_CharacterWindow->m_CurrentCharacterID = CharacterManager::get()->getPlayerCharacterID();
				UIManager::get()->m_CharacterWindow->checkIfPlayerCharacter();
				UIManager::get()->m_CharacterWindow->updateInfo();
				UIManager::get()->m_CharacterWindow->openWindow();
			}
		}
	}

	void updateSprites()
	{
		updateSprite(m_InstigatorSprite, m_InstigatorTexture, sf::Vector2f(m_InstigatorCharacterPosition.x, m_InstigatorCharacterPosition.y));
		updateSprite(m_PlayerCharacterSprite, m_PlayerCharacterTexture, sf::Vector2f(m_PlayerCharacterPosition.x, m_PlayerCharacterPosition.y));
		updateSprite(m_MessageTypeSprite, m_MessageTypeTexture, sf::Vector2f(m_MessageTypePosition.x, m_MessageTypePosition.y), m_SpriteSize / 2);
	}

	void updateSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, int spriteSize = m_SpriteSize)
	{
		sprite.setTexture(texture, true);
		sprite.setPosition(Window::getWindow()->mapPixelToCoords(sf::Vector2i((int)position.x, (int)position.y)));

		sf::FloatRect localSize = sprite.getLocalBounds();

		sprite.setScale(spriteSize / localSize.width, spriteSize / localSize.height);

		m_Window->draw(sprite);
	}
};