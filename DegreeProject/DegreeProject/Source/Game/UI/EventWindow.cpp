#include "Game/UI/EventWindow.h"
#include <SFML/Graphics.hpp>
#include "Engine/Window.h"
#include "Engine/InputHandler.h"
#include "Game/Systems/CharacterManager.h"
#include "Game/DiplomacyManager.h"
#include "Engine/AssetHandler.h"
#include <sstream>
#include <iomanip>
#include "Game/Data/UIType.h"
#include "Game/UI/UIManager.h"
#include "Game/UI/CharacterWindow.h"

EventWindow::EventWindow(UIID ID, sf::Font font, CharacterID instigatorID, CharacterID subjectID, UIType type, float goldAmount, std::string buildingName, std::string regionName)
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
	setSprite(m_SubjectSprite, m_SubjectTexture, { m_PositionX + m_SizeX - m_SpriteSize * 1.5f, m_PositionY + m_SpriteSize * 0.5f });

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
	setSprite(m_InstigatorSprite, m_InstigatorTexture, { m_PositionX + m_SpriteSize * 0.5f, m_PositionY + m_SpriteSize * 0.5f });

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
			break;
		}
		case UIType::AllianceBroken:
		{
			m_MessageTypeTexture = assetHandler.getTextureAtPath("Assets/Graphics/Alliance.png");
			stream.str(std::string());
			stream.clear();
			stream << "You're alliance with " << subject.m_Name << " is broken.";
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
			break;
		}
		case UIType::PeaceRequest:
		case UIType::PeaceAccepted:
		case UIType::PeaceDeclined:
		{
			m_MessageTypeTexture = assetHandler.getTextureAtPath("Assets/Graphics/Peace.png");
			if (m_MessageType == UIType::PeaceRequest)
			{
				stream << "\nwants a white peace.\n\n               Do you accept?";
			}
			else if (m_MessageType == UIType::PeaceAccepted)
			{
				stream << "\naccepted your peace offer!";
			}
			else
			{
				stream << "\ndeclined your peace offer!";
			}
			break;
		}
		case UIType::TruceMessage:
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
			else if (m_MessageType == UIType::TriedToDeclareWarOnAlly)
			{
				stream.str(std::string());
				stream.clear();
				stream << "You can't declare war on an ally!";
			}
			else
			{
				stream.str(std::string());
				stream.clear();
				stream << "You can't declare war on\n";
				if (subject.m_Gender == Gender::Male)
				{
					stream << m_MaleTitles[(unsigned int)subject.m_CharacterTitle] << subject.m_Name << "\nYou have a truce!";
				}
				else
				{
					stream << m_FemaleTitles[(unsigned int)subject.m_CharacterTitle] << subject.m_Name << "\nYou have a truce!";
				}
			}
			break;
		}
		case UIType::BuildingMessage:
		case UIType::CannotAffordMessage:
		case UIType::Gift:
		{
			m_MessageTypeTexture = assetHandler.getTextureAtPath("Assets/Graphics/Coins.png");
			if (m_MessageType == UIType::CannotAffordMessage)
			{
				stream.str(std::string());
				stream.clear();
				stream << "You can't afford the cost\nof " << std::fixed << std::setprecision(1) << goldAmount << " gold to do this.";
			}
			else if(m_MessageType == UIType::Gift)
			{
				stream << "\nsent you a gift of " << std::fixed << std::setprecision(1) << goldAmount << " gold!";
			}
			else
			{
				stream.str(std::string());
				stream.clear();
				stream << "Started construction of a " << buildingName << "\nin " << regionName << " for a cost of\n" << std::fixed << std::setprecision(1) << goldAmount << " gold.";
			}
			break;
		}
		case UIType::Pregnant:
		{
			m_MessageTypeTexture = assetHandler.getTextureAtPath("Assets/Graphics/Pregnant.png");
			stream << "\njust got pregnant!";
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
				}
				else
				{
					m_MessageTypeTexture = assetHandler.getTextureAtPath("Assets/Graphics/BabyFemale.png");
					stream << "\njust gave birth to a healthy baby girl!" << "\nHer name is " << child.m_Name << ".";
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
			break;
		}
		case UIType::Death:
		{
			m_MessageTypeTexture = assetHandler.getTextureAtPath("Assets/Graphics/Dead.png");
			stream << "\nof the " << instigator.m_KingdomName << "\njust died!";
			break;
		}
		default:
		{
			break;
		}
	}
	setSprite(m_MessageTypeSprite, m_MessageTypeTexture, { m_PositionX + m_SizeX * 0.5f - m_SpriteSize * 0.25f, m_PositionY + m_SpriteSize * 0.75f }, (unsigned int)(m_SpriteSize * 0.5f));
	setText(m_MessageText, m_Font, m_CharacterSize, m_OwnerColor, { m_PositionX + m_SpriteSize * 0.5f, m_PositionY + m_SpriteSize * 1.6f }, stream.str().c_str());
	stream.str(std::string());
	stream.clear();

	setShape(m_WindowShape, m_FillColor, m_OwnerColor, m_OutlineThickness, { m_SizeX, m_SizeY }, { m_PositionX, m_PositionY });

	setShape(m_InstigatorShape, m_TransparentColor, m_InstigatorColor, m_OutlineThickness * 0.5f, { m_SpriteSize, m_SpriteSize }, m_InstigatorSprite.getPosition());

	setShape(m_SubjectShape, m_TransparentColor, m_SubjectColor, m_OutlineThickness * 0.5f, { m_SpriteSize, m_SpriteSize }, m_SubjectSprite.getPosition());

	if (m_MessageType == UIType::MarriageRequest || m_MessageType == UIType::AllianceRequest || m_MessageType == UIType::PeaceRequest || m_MessageType == UIType::CallToArmsRequest)
	{
		setShape(m_AgreeShape, m_TransparentColor, m_AgreeColor, m_OutlineThickness * 0.5f, { m_SpriteSize * 1.25f, m_SpriteSize * 0.5f }, { m_PositionX + m_SizeX - m_SpriteSize * 1.75f, m_PositionY + m_SizeY - m_SpriteSize });
		setText(m_AgreeText, m_Font, m_CharacterSize, m_AgreeColor, m_AgreeShape.getPosition(), m_AgreeString);
	}

	setShape(m_DismissShape, m_TransparentColor, m_DismissColor, m_OutlineThickness * 0.5f, { m_SpriteSize * 1.25f, m_SpriteSize * 0.5f }, { m_PositionX + m_SpriteSize * 0.5f, m_PositionY + m_SizeY - m_SpriteSize });
	setText(m_DismissText, m_Font, m_CharacterSize, m_DismissColor, m_DismissShape.getPosition(), m_DismissString);

	Time::pauseGame();
}

void EventWindow::update()
{
	clickButton();

	if (m_MovingWindow)
	{
		moveWindow();
	}
	handleWindow();
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

void EventWindow::handleWindow()
{
	if (InputHandler::m_Inputs[EnterPressed])
	{
		InputHandler::m_Inputs[EnterPressed] = false;
		m_AgreeShape.setFillColor(m_AgreeShape.getOutlineColor());
		m_AgreeText.setFillColor(m_FillColor);
	}
	else if (InputHandler::m_Inputs[EnterReleased])
	{
		InputHandler::m_Inputs[EnterReleased] = false;
		m_AgreeShape.setFillColor(m_TransparentColor);
		m_AgreeText.setFillColor(m_AgreeShape.getOutlineColor());
		acceptRequest();
		closeWindow();
	}
	else if(InputHandler::m_Inputs[BackSpacePressed])
	{
		InputHandler::m_Inputs[BackSpacePressed] = false;
		m_DismissShape.setFillColor(m_DismissShape.getOutlineColor());
		m_DismissText.setFillColor(m_FillColor);
	}
	else if (InputHandler::m_Inputs[BackSpaceReleased])
	{
		m_DismissShape.setFillColor(m_TransparentColor);
		m_DismissText.setFillColor(m_DismissShape.getOutlineColor());
		InputHandler::m_Inputs[BackSpaceReleased] = false;
		dismissRequest();
		closeWindow();
	}
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
			DiplomacyManager::get().breakAlliance(m_InstigatorID, m_SubjectID);
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
				&& !subject.m_Dead && subject.m_Spouse == INVALID_CHARACTER_ID
				&& DiplomacyManager::get().getWarHandleAgainst(m_InstigatorID, m_SubjectID) == -1)
			{
				CharacterManager::get().onMarriage(m_InstigatorID, m_SubjectID);
				DiplomacyManager::get().createAlliance(m_InstigatorID, m_SubjectID);
			}
			break;
		}
		case UIType::AllianceRequest:
		{
			if (!instigator.m_Dead && !subject.m_Dead && DiplomacyManager::get().getWarHandleAgainst(m_InstigatorID, m_SubjectID) == -1)
			{
				DiplomacyManager::get().createAlliance(m_InstigatorID, m_SubjectID);
			}
			break;
		}
		case UIType::PeaceRequest:
		{
			if (DiplomacyManager::get().getWarHandleAgainst(m_InstigatorID, m_SubjectID) != -1)
			{
				CharacterManager::get().onWarEnded(m_InstigatorID, m_SubjectID);
			}
			break;
		}
		case UIType::CallToArmsRequest:
		{
			Character& playerCharacter = CharacterManager::get().getPlayerCharacter();
			DiplomacyManager& warManager = DiplomacyManager::get();

			for (int handle : DiplomacyManager::get().getWarHandlesOfCharacter(m_InstigatorID))
			{
				if (warManager.isAttacker(handle, m_InstigatorID))
				{
					warManager.addAttacker(handle, playerCharacter.m_CharacterID);
				}

				else if (warManager.isDefender(handle, m_InstigatorID))
				{
					warManager.addDefender(handle, playerCharacter.m_CharacterID);
				}

				UIManager::get().createWarIcon(warManager.getAttacker(handle), warManager.getDefender(handle));
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
		if (m_DismissShape.getGlobalBounds().contains(m_MousePosition.x, m_MousePosition.y))
		{
			InputHandler::setLeftMouseReleased(false);
			m_DismissShape.setFillColor(m_DismissShape.getOutlineColor());
			m_DismissText.setFillColor(m_FillColor);
		}
		else if (m_AgreeShape.getGlobalBounds().contains(m_MousePosition.x, m_MousePosition.y))
		{
			InputHandler::setLeftMouseReleased(false);
			m_AgreeShape.setFillColor(m_AgreeShape.getOutlineColor());
			m_AgreeText.setFillColor(m_FillColor);
		}
		else if (m_WindowShape.getGlobalBounds().contains(m_MousePosition.x, m_MousePosition.y))
		{
			InputHandler::setLeftMouseClicked(false);
			m_MovingWindow = true;
		}
	}
	if (InputHandler::getLeftMouseReleased())
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		m_DismissShape.setFillColor(m_TransparentColor);
		m_DismissText.setFillColor(m_DismissShape.getOutlineColor());
		m_AgreeShape.setFillColor(m_TransparentColor);
		m_AgreeText.setFillColor(m_AgreeShape.getOutlineColor());
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

void EventWindow::setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position)
{
	shape.setFillColor(fillColor);
	shape.setOutlineColor(outlineColor);
	shape.setOutlineThickness(outlineThickness);
	shape.setSize(size);
	shape.setPosition(position);
}

void EventWindow::setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Vector2f position, const char* string)
{
	text.setFont(font);
	text.setCharacterSize(characterSize);
	text.setFillColor(fillColor);
	text.setPosition(position);
	text.setString(string);
}

void EventWindow::setSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, unsigned int spriteSize)
{
	sprite.setTexture(texture, true);
	sprite.setScale(spriteSize / sprite.getLocalBounds().width, spriteSize / sprite.getLocalBounds().height);
	sprite.setPosition(position);
}