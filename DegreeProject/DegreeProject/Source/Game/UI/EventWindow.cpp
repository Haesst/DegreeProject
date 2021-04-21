#include "Game/UI/EventWindow.h"
#include <SFML/Graphics.hpp>
#include "Engine/Window.h"
#include "Engine/InputHandler.h"
#include "Game/Systems/CharacterManager.h"
#include "Game/WarManager.h"
#include "Engine/AssetHandler.h"
#include <sstream>
#include <iomanip>
#include "Game/Data/UIType.h"
#include "Game/UI/UIManager.h"
#include "Game/UI/CharacterWindow.h"

EventWindow::EventWindow(UIID ID, sf::Font font, CharacterID instigatorID, CharacterID subjectID, UIType type, float giftAmount)
{
	m_OwnedUIWindow = ID;
	m_Font = font;
	m_InstigatorID = instigatorID;
	m_SubjectID = subjectID;
	m_MessageType = type;

	m_Window = Window::getWindow();

	Character& playerCharacter = CharacterManager::get().getPlayerCharacter();
	m_OwnerColor = playerCharacter.m_RegionColor;

	Character& instigator = CharacterManager::get().getCharacter(m_InstigatorID);
	m_InstigatorColor = instigator.m_RegionColor;

	Character& subject = CharacterManager::get().getCharacter(m_SubjectID);
	m_SubjectColor = subject.m_RegionColor;

	AssetHandler& assetHandler = AssetHandler::get();

	m_PositionX = m_Window->getSize().x * 0.5f - m_OutlineThickness - m_SizeX * 0.5f;
	m_PositionY = m_Window->getSize().y * 0.5f - m_OutlineThickness - m_SizeY * 0.5f;

	if (subject.m_Gender == Gender::Male)
	{
		m_SubjectTexture = assetHandler.getTextureAtPath("Assets/Graphics/Harold.jpg");
	}
	else
	{
		m_SubjectTexture = assetHandler.getTextureAtPath("Assets/Graphics/Harriet.jpg");
	}
	m_SubjectSprite.setTexture(m_SubjectTexture);
	m_SubjectSprite.setPosition(m_PositionX + m_SizeX - m_SpriteSize * 1.5f, m_PositionY + m_SpriteSize * 0.5f);
	m_SubjectSprite.setScale(m_SpriteSize / m_SubjectSprite.getLocalBounds().width, m_SpriteSize / m_SubjectSprite.getLocalBounds().height);

	std::stringstream stream;
	if (instigator.m_Gender == Gender::Male)
	{
		m_InstigatorTexture = assetHandler.getTextureAtPath("Assets/Graphics/Harold.jpg");
		stream << m_MaleTitles[(unsigned int)instigator.m_CharacterTitle];
	}
	else
	{
		m_InstigatorTexture = assetHandler.getTextureAtPath("Assets/Graphics/Harriet.jpg");
		stream << m_FemaleTitles[(unsigned int)instigator.m_CharacterTitle];
	}
	m_InstigatorSprite.setTexture(m_InstigatorTexture);
	m_InstigatorSprite.setPosition(m_PositionX + m_SpriteSize * 0.5f, m_PositionY + m_SpriteSize * 0.5f);
	m_InstigatorSprite.setScale(m_SpriteSize / m_InstigatorSprite.getLocalBounds().width, m_SpriteSize / m_InstigatorSprite.getLocalBounds().height);

	stream << instigator.m_Name;
	switch (m_MessageType)
	{
		case UIType::MarriageRequest:
		case UIType::MarriageAccepted:
		case UIType::MarriageDeclined:
		{
			m_MessageTypeTexture = assetHandler.getTextureAtPath("Assets/Graphics/Married.png");
			if (m_MessageType == UIType::MarriageRequest)
			{
				stream << "\nwants to marry you.\n\n               Do you accept?";
			}
			else if (m_MessageType == UIType::MarriageAccepted)
			{
				stream << "\naccepted your marriage proposal!";
			}
			else
			{
				stream << "\ndeclined your marriage proposal!";
			}
			m_MessageText.setString(stream.str());
			break;
		}
		case UIType::AllianceRequest:
		case UIType::AllianceAccepted:
		case UIType::AllianceDeclined:
		{
			m_MessageTypeTexture = assetHandler.getTextureAtPath("Assets/Graphics/Alliance.png");
			if (m_MessageType == UIType::AllianceRequest)
			{
				stream << "\nwants to ally you.\n\n               Do you accept?";
			}
			else if (m_MessageType == UIType::AllianceAccepted)
			{
				stream << "\naccepted your alliance proposal!";
			}
			else
			{
				stream << "\ndeclined your alliance proposal!";
			}
			m_MessageText.setString(stream.str());
			break;
		}
		case UIType::AllianceBroken:
		{
			m_MessageTypeTexture = assetHandler.getTextureAtPath("Assets/Graphics/Alliance.png");

			stream.str(std::string());
			stream.clear();
			stream << "You're alliance with " << subject.m_Name << " is broken.";
			m_MessageText.setString(stream.str());

			break;
		}
		case UIType::CallToArmsRequest:
		case UIType::CallToArmsAccepted:
		case UIType::CallToArmsRejected:
		{
			m_MessageTypeTexture = assetHandler.getTextureAtPath("Assets/Graphics/Alliance.png");
			if (m_MessageType == UIType::CallToArmsRequest)
			{
				stream << "\nis calling you to arms.\n\n               Do you honor it?";
			}
			else if (m_MessageType == UIType::CallToArmsAccepted)
			{
				stream << "\naccepted your call to arms!";
			}
			else
			{
				stream << "\ndeclined your call to arms!";
			}
			m_MessageText.setString(stream.str());
			break;
		}
		case UIType::PeaceRequest:
		case UIType::PeaceAccepted:
		case UIType::PeaceDeclined:
		{
			m_MessageTypeTexture = assetHandler.getTextureAtPath("Assets/Graphics/Peace.png");
			if (m_MessageType == UIType::PeaceRequest)
			{
				stream << "\nwants peace.\n\n               Do you accept?";
			}
			else if (m_MessageType == UIType::PeaceAccepted)
			{
				stream << "\naccepted your peace offer!";
			}
			else
			{
				stream << "\ndeclined your peace offer!";
			}
			m_MessageText.setString(stream.str());
			break;
		}
		case UIType::TriedToDeclareWarOnAlly:
		case UIType::WarDeclaration:
		{
			m_MessageTypeTexture = assetHandler.getTextureAtPath("Assets/Graphics/War.png");
			if (m_MessageType == UIType::WarDeclaration)
			{
				if (instigatorID == playerCharacter.m_CharacterID)
				{
					if (subject.m_Gender == Gender::Male)
					{
						stream << "\ndeclared war on\n" << m_MaleTitles[(unsigned int)subject.m_CharacterTitle] << subject.m_Name << "!";
					}
					else
					{
						stream << "\ndeclared war on\n" << m_FemaleTitles[(unsigned int)subject.m_CharacterTitle] << subject.m_Name << "!";
					}
				}
				else
				{
					stream << "\ndeclared war on you!";
				}
			}
			else
			{
				stream.str(std::string());
				stream.clear();
				stream << "You can't declare war on an ally!";
			}
			m_MessageText.setString(stream.str());
			break;
		}
		case UIType::Gift:
		{
			m_MessageTypeTexture = assetHandler.getTextureAtPath("Assets/Graphics/Coins.png");
			stream << "\nsent you a gift of " << std::fixed << std::setprecision(1) << giftAmount << " gold!";
			m_MessageText.setString(stream.str());
			break;
		}
		case UIType::Pregnant:
		{
			m_MessageTypeTexture = assetHandler.getTextureAtPath("Assets/Graphics/Pregnant.png");
			stream << "\njust got pregnant!";
			m_MessageText.setString(stream.str());
			break;
		}
		case UIType::ChildBirth:
		{
			if (instigator.m_Children.size() > 0)
			{
				Character& child = CharacterManager::get().getCharacter(instigator.m_Children.back());
				if (child.m_Gender == Gender::Male)
				{
					m_MessageTypeTexture = assetHandler.getTextureAtPath("Assets/Graphics/BabyMale.png");
					stream << "\njust gave birth to a healthy baby boy!" << "\nHis name is " << child.m_Name << ".";
					m_MessageText.setString(stream.str());
				}
				else
				{
					m_MessageTypeTexture = assetHandler.getTextureAtPath("Assets/Graphics/BabyFemale.png");
					stream << "\njust gave birth to a healthy baby girl!" << "\nHer name is " << child.m_Name << ".";
					m_MessageText.setString(stream.str());
				}
			}
			break;
		}
		case UIType::AssassinationSuccess:
		{
			m_MessageTypeTexture = assetHandler.getTextureAtPath("Assets/Graphics/Assassinate.png");
			stream.str(std::string());
			stream.clear();
			if (subject.m_Gender == Gender::Male)
			{
				stream << m_MaleTitles[(unsigned int)subject.m_CharacterTitle];
			}
			else
			{
				stream << m_FemaleTitles[(unsigned int)subject.m_CharacterTitle];
			}
			stream << subject.m_Name << "\ndied under suspicious circumstances.";
			m_MessageText.setString(stream.str());
			break;
		}
		case UIType::AssassinationFailure:
		{
			m_MessageTypeTexture = assetHandler.getTextureAtPath("Assets/Graphics/Assassinate.png");
			stream.str(std::string());
			stream.clear();
			if (subject.m_Gender == Gender::Male)
			{
				stream << m_MaleTitles[(unsigned int)subject.m_CharacterTitle];
			}
			else
			{
				stream << m_FemaleTitles[(unsigned int)subject.m_CharacterTitle];
			}
			stream << subject.m_Name << "\nevaded an assassination!";
			m_MessageText.setString(stream.str());
			break;
		}
		case UIType::Death:
		{
			m_MessageTypeTexture = assetHandler.getTextureAtPath("Assets/Graphics/Dead.png");
			stream << "\njust died!";
			m_MessageText.setString(stream.str());
			break;
		}
		default:
		{
			break;
		}
	}
	m_MessageTypeSprite.setTexture(m_MessageTypeTexture);
	m_MessageTypeSprite.setPosition(m_PositionX + m_SizeX * 0.5f - m_SpriteSize * 0.25f, m_PositionY + m_SpriteSize * 0.75f);
	m_MessageTypeSprite.setScale(m_SpriteSize * 0.5f / m_MessageTypeSprite.getLocalBounds().width, m_SpriteSize * 0.5f / m_MessageTypeSprite.getLocalBounds().height);

	m_WindowShape.setFillColor(m_FillColor);
	m_WindowShape.setOutlineThickness(m_OutlineThickness);
	m_WindowShape.setSize(sf::Vector2f(m_SizeX, m_SizeY));
	m_WindowShape.setOutlineColor(m_OwnerColor);
	m_WindowShape.setPosition(m_PositionX, m_PositionY);

	m_InstigatorShape.setFillColor(sf::Color::Transparent);
	m_InstigatorShape.setOutlineThickness(m_OutlineThickness * 0.5f);
	m_InstigatorShape.setOutlineColor(instigator.m_RegionColor);
	m_InstigatorShape.setSize(sf::Vector2f(m_SpriteSize, m_SpriteSize));
	m_InstigatorShape.setPosition(m_InstigatorSprite.getPosition());

	m_SubjectShape.setFillColor(sf::Color::Transparent);
	m_SubjectShape.setOutlineThickness(m_OutlineThickness * 0.5f);
	m_SubjectShape.setOutlineColor(m_SubjectColor);
	m_SubjectShape.setSize(sf::Vector2f(m_SpriteSize, m_SpriteSize));
	m_SubjectShape.setPosition(m_SubjectSprite.getPosition());

	if (m_MessageType == UIType::MarriageRequest || m_MessageType == UIType::AllianceRequest || m_MessageType == UIType::PeaceRequest || m_MessageType == UIType::CallToArmsRequest)
	{
		m_AgreeShape.setSize(sf::Vector2f(m_SpriteSize * 1.25f, m_SpriteSize * 0.5f));
		m_AgreeShape.setFillColor(sf::Color::Transparent);
		m_AgreeShape.setOutlineThickness(m_OutlineThickness * 0.5f);
		m_AgreeShape.setOutlineColor(m_AgreeColor);
		m_AgreeShape.setPosition(m_PositionX + m_SizeX - m_SpriteSize * 1.75f, m_PositionY + m_SizeY - m_SpriteSize);
		m_AgreeText.setFont(m_Font);
		m_AgreeText.setCharacterSize(m_CharacterSize);
		m_AgreeText.setFillColor(m_AgreeColor);
		m_AgreeText.setString(m_AgreeString);
		m_AgreeText.setPosition(m_AgreeShape.getPosition());
	}

	m_DismissShape.setSize(sf::Vector2f(m_SpriteSize * 1.25f, m_SpriteSize * 0.5f));
	m_DismissShape.setFillColor(sf::Color::Transparent);
	m_DismissShape.setOutlineThickness(m_OutlineThickness * 0.5f);
	m_DismissShape.setOutlineColor(m_DismissColor);
	m_DismissShape.setPosition(m_PositionX + m_SpriteSize * 0.5f, m_PositionY + m_SizeY - m_SpriteSize);
	m_DismissText.setFont(m_Font);
	m_DismissText.setCharacterSize(m_CharacterSize);
	m_DismissText.setFillColor(m_DismissColor);
	m_DismissText.setString(m_DismissString);
	m_DismissText.setPosition(m_DismissShape.getPosition());

	m_MessageText.setFont(m_Font);
	m_MessageText.setCharacterSize(m_CharacterSize);
	m_MessageText.setFillColor(m_OwnerColor);
	m_MessageText.setPosition(m_PositionX + m_SpriteSize * 0.5f, m_PositionY + m_SpriteSize * 1.6f);

	Time::pauseGame();
}

void EventWindow::update()
{
	clickButton();

	if (m_MovingWindow)
	{
		moveWindow();
	}
}

void EventWindow::render()
{
	m_Window->draw(m_WindowShape);
	m_Window->draw(m_InstigatorShape);
	m_Window->draw(m_InstigatorSprite);
	m_Window->draw(m_SubjectShape);
	m_Window->draw(m_SubjectSprite);
	m_Window->draw(m_MessageTypeSprite);
	m_Window->draw(m_MessageText);
	if (m_MessageType == UIType::MarriageRequest || m_MessageType == UIType::AllianceRequest 
	 || m_MessageType == UIType::PeaceRequest || m_MessageType == UIType::CallToArmsRequest)
	{
		m_Window->draw(m_AgreeShape);
		m_Window->draw(m_AgreeText);
	}
	m_Window->draw(m_DismissShape);
	m_Window->draw(m_DismissText);
}

void EventWindow::moveWindow()
{
	Vector2D mousePosition = InputHandler::getUIMousePosition();
	Vector2D diff = m_MousePosition - mousePosition;
	m_MousePosition = mousePosition;

	m_WindowShape.setPosition(m_WindowShape.getPosition().x - diff.x, m_WindowShape.getPosition().y - diff.y);
	m_InstigatorShape.setPosition(m_InstigatorShape.getPosition().x - diff.x, m_InstigatorShape.getPosition().y - diff.y);
	m_InstigatorSprite.setPosition(m_InstigatorSprite.getPosition().x - diff.x, m_InstigatorSprite.getPosition().y - diff.y);
	m_SubjectShape.setPosition(m_SubjectShape.getPosition().x - diff.x, m_SubjectShape.getPosition().y - diff.y);
	m_SubjectSprite.setPosition(m_SubjectSprite.getPosition().x - diff.x, m_SubjectSprite.getPosition().y - diff.y);
	m_MessageTypeSprite.setPosition(m_MessageTypeSprite.getPosition().x - diff.x, m_MessageTypeSprite.getPosition().y - diff.y);
	m_MessageText.setPosition(m_MessageText.getPosition().x - diff.x, m_MessageText.getPosition().y - diff.y);
	m_DismissShape.setPosition(m_DismissShape.getPosition().x - diff.x, m_DismissShape.getPosition().y - diff.y);
	m_DismissText.setPosition(m_DismissText.getPosition().x - diff.x, m_DismissText.getPosition().y - diff.y);
	m_AgreeShape.setPosition(m_AgreeShape.getPosition().x - diff.x, m_AgreeShape.getPosition().y - diff.y);
	m_AgreeText.setPosition(m_AgreeText.getPosition().x - diff.x, m_AgreeText.getPosition().y - diff.y);
}

void EventWindow::closeWindow()
{
	m_Dismissed = true;
	Time::unpauseGame();
}

void EventWindow::dismissRequest()
{
	switch (m_MessageType)
	{
		case UIType::CallToArmsRequest:
		{
			WarManager::get().breakAlliance(m_InstigatorID, m_SubjectID);
			break;
		}
		default:
		{
			break;
		}
	}
}

void EventWindow::acceptRequest()
{
	Character& instigator = CharacterManager::get().getCharacter(m_InstigatorID);
	Character& subject = CharacterManager::get().getCharacter(m_SubjectID);
	switch (m_MessageType)
	{
		case UIType::MarriageRequest:
		{
			if (!instigator.m_Dead && instigator.m_Spouse == INVALID_CHARACTER_ID
				&& !subject.m_Dead && subject.m_Spouse == INVALID_CHARACTER_ID)
			{
				CharacterManager::get().onMarriage(m_InstigatorID, m_SubjectID);
			}
			break;
		}
		case UIType::AllianceRequest:
		{
			if (!instigator.m_Dead && !subject.m_Dead)
			{
				WarManager::get().createAlliance(m_InstigatorID, m_SubjectID);
			}
			break;
		}
		case UIType::PeaceRequest:
		{
			CharacterManager::get().onWarEnded(m_InstigatorID, m_SubjectID);
			break;
		}
		case UIType::CallToArmsRequest:
		{
			Character& playerCharacter = CharacterManager::get().getPlayerCharacter();

			for (int handle : WarManager::get().getWarHandlesOfCharacter(m_InstigatorID))
			{
				War* currentWar = WarManager::get().getWar(handle);

				if (currentWar == nullptr)
				{
					continue;
				}

				if (currentWar->isAttacker(m_InstigatorID))
				{
					currentWar->addAttacker(playerCharacter.m_CharacterID);
				}

				else if (currentWar->isDefender(m_InstigatorID))
				{
					currentWar->addDefender(playerCharacter.m_CharacterID);
				}

				UIManager::get().createWarIcon(currentWar->getAttacker(), currentWar->getDefender());
			}
			break;
		}
		default:
		{
			break;
		}
	}
}

void EventWindow::clickButton()
{
	if (InputHandler::getLeftMouseClicked())
	{
		m_MousePosition = InputHandler::getUIMousePosition();
		if (m_WindowShape.getGlobalBounds().contains(m_MousePosition.x, m_MousePosition.y))
		{
			InputHandler::setLeftMouseClicked(false);
			m_MovingWindow = true;
		}
	}
	if (InputHandler::getLeftMouseReleased())
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		if (m_DismissShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setLeftMouseReleased(false);
			dismissRequest();
			closeWindow();
		}
		else if (m_AgreeShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setLeftMouseReleased(false);
			acceptRequest();
			closeWindow();
		}
		if (m_MovingWindow)
		{
			m_MovingWindow = false;
		}
	}
	else if (InputHandler::getRightMouseReleased())
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		CharacterWindow& characterWindow = *UIManager::get().m_CharacterWindow;
		if (m_InstigatorShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setRightMouseReleased(false);
			characterWindow.m_CurrentCharacterID = m_InstigatorID;
			characterWindow.checkIfPlayerCharacter();
			characterWindow.updateInfo();
			characterWindow.openWindow();
		}
		else if (m_SubjectShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setRightMouseReleased(false);
			characterWindow.m_CurrentCharacterID = m_SubjectID;
			characterWindow.checkIfPlayerCharacter();
			characterWindow.updateInfo();
			characterWindow.openWindow();
		}
	}
}