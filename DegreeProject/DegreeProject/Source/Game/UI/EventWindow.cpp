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

	m_MaleTitles[(unsigned int)Title::Emperor] = "Emperor ";
	m_MaleTitles[(unsigned int)Title::King] = "King ";
	m_MaleTitles[(unsigned int)Title::Duke] = "Duke ";
	m_MaleTitles[(unsigned int)Title::Count] = "Count ";
	m_MaleTitles[(unsigned int)Title::Baron] = "Baron ";

	m_FemaleTitles[(unsigned int)Title::Emperor] = "Empress ";
	m_FemaleTitles[(unsigned int)Title::King] = "Queen ";
	m_FemaleTitles[(unsigned int)Title::Duke] = "Duchess ";
	m_FemaleTitles[(unsigned int)Title::Count] = "Countess ";
	m_FemaleTitles[(unsigned int)Title::Baron] = "Baroness ";

	m_Window = Window::getWindow();

	Character& playerCharacter = CharacterManager::get()->getPlayerCharacter();
	m_OwnerColor = playerCharacter.m_RegionColor;

	Character& instigator = CharacterManager::get()->getCharacter(m_InstigatorID);
	m_InstigatorColor = instigator.m_RegionColor;

	Character& subject = CharacterManager::get()->getCharacter(m_SubjectID);
	m_SubjectColor = subject.m_RegionColor;

	if (subject.m_Gender == Gender::Male)
	{
		m_SubjectTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Harold.jpg");
	}
	else
	{
		m_SubjectTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Harriet.jpg");
	}

	std::stringstream stream;
	if (instigator.m_Gender == Gender::Male)
	{
		m_InstigatorTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Harold.jpg");
		stream << m_MaleTitles[(unsigned int)instigator.m_CharacterTitle];
	}
	else
	{
		m_InstigatorTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Harriet.jpg");
		stream << m_FemaleTitles[(unsigned int)instigator.m_CharacterTitle];
	}
	stream << instigator.m_Name;
	switch (m_MessageType)
	{
		case UIType::MarriageRequest:
		case UIType::MarriageAccepted:
		case UIType::MarriageDeclined:
		{
			m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Married.png");
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
			m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Alliance.png");
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
			m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Alliance.png");

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
			m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Alliance.png");
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
			m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Peace.png");
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
		case UIType::WarDeclaration:
		{
			m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/War.png");
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
			m_MessageText.setString(stream.str());
			break;
		}
		case UIType::TriedToDeclareWarOnAlly:
		{
			m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/War.png");

			stream.str(std::string());
			stream.clear();
			stream << "You can't declare war on an ally!";
			m_MessageText.setString(stream.str());

			break;
		}
		case UIType::Gift:
		{
			m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Coins.png");
			stream << "\nsent you a gift of " << std::fixed << std::setprecision(1) << giftAmount << " gold!";
			m_MessageText.setString(stream.str());
			break;
		}
		case UIType::Pregnant:
		{
			m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Pregnant.png");
			stream << "\njust got pregnant!";
			m_MessageText.setString(stream.str());
			break;
		}
		case UIType::ChildBirth:
		{
			if (instigator.m_Children.size() > 0)
			{
				Character& child = CharacterManager::get()->getCharacter(instigator.m_Children.back());
				if (child.m_Gender == Gender::Male)
				{
					m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/BabyMale.png");
					stream << "\njust gave birth to a healthy baby boy!" << "\nHis name is " << child.m_Name << ".";
					m_MessageText.setString(stream.str());
				}
				else
				{
					m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/BabyFemale.png");
					stream << "\njust gave birth to a healthy baby girl!" << "\nHer name is " << child.m_Name << ".";
					m_MessageText.setString(stream.str());
				}
			}
			break;
		}
		case UIType::AssassinationSuccess:
		{
			m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Assassinate.png");
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
			m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Assassinate.png");
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
			m_MessageTypeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Dead.png");
			stream << "\njust died!";
			m_MessageText.setString(stream.str());
			break;
		}
		default:
		{
			break;
		}
	}

	m_PositionX = m_Window->getSize().x * 0.5f - m_OutlineThickness - m_SizeX * 0.5f;
	m_PositionY = m_Window->getSize().y * 0.5f - m_OutlineThickness - m_SizeY * 0.5f;

	m_WindowShape.setFillColor(m_FillColor);
	m_WindowShape.setOutlineThickness(m_OutlineThickness);
	m_WindowShape.setSize(sf::Vector2f(m_SizeX, m_SizeY));
	m_WindowShape.setOutlineColor(m_OwnerColor);
	m_WindowShape.setPosition(sf::Vector2f(m_PositionX, m_PositionY));

	m_InstigatorPosition = sf::Vector2f(m_PositionX + m_SpriteSize * 0.5f, m_PositionY + m_SpriteSize * 0.5f);
	m_InstigatorShape.setFillColor(sf::Color::Transparent);
	m_InstigatorShape.setOutlineThickness(m_OutlineThickness * 0.5f);
	m_InstigatorShape.setOutlineColor(instigator.m_RegionColor);
	m_InstigatorShape.setSize(sf::Vector2f(m_SpriteSize, m_SpriteSize));
	m_InstigatorShape.setPosition(sf::Vector2f(m_InstigatorPosition));

	m_MessageTypePosition = sf::Vector2f(m_PositionX + m_SizeX * 0.5f - m_SpriteSize * 0.25f, m_PositionY + m_SpriteSize * 0.75f);

	m_SubjectPosition = sf::Vector2f(m_PositionX + m_SizeX - m_SpriteSize * 1.5f, m_PositionY + m_SpriteSize * 0.5f);
	m_SubjectShape.setFillColor(sf::Color::Transparent);
	m_SubjectShape.setOutlineThickness(m_OutlineThickness * 0.5f);
	m_SubjectShape.setOutlineColor(m_SubjectColor);
	m_SubjectShape.setSize(sf::Vector2f(m_SpriteSize, m_SpriteSize));
	m_SubjectShape.setPosition(sf::Vector2f(m_SubjectPosition));

	if (m_MessageType == UIType::MarriageRequest || m_MessageType == UIType::AllianceRequest || m_MessageType == UIType::PeaceRequest || m_MessageType == UIType::CallToArmsRequest)
	{
		m_AgreePosition = sf::Vector2f(m_PositionX + m_SizeX - m_SpriteSize * 1.75f, m_PositionY + m_SizeY - m_SpriteSize);
		m_AgreeShape.setSize(sf::Vector2f(m_SpriteSize * 1.25f, m_SpriteSize * 0.5f));
		m_AgreeShape.setFillColor(sf::Color::Transparent);
		m_AgreeShape.setOutlineThickness(m_OutlineThickness * 0.5f);
		m_AgreeShape.setOutlineColor(m_AgreeColor);
		m_AgreeShape.setPosition(sf::Vector2f(m_AgreePosition));
		m_AgreeText.setFont(m_Font);
		m_AgreeText.setCharacterSize((int)(m_CharacterSize * 0.5f));
		m_AgreeText.setFillColor(m_AgreeColor);
		m_AgreeText.setString(m_AgreeString);
		m_AgreeText.setPosition(sf::Vector2f(m_AgreePosition));
	}

	m_DismissPosition = sf::Vector2f(m_PositionX + m_SpriteSize * 0.5f, m_PositionY + m_SizeY - m_SpriteSize);
	m_DismissShape.setSize(sf::Vector2f(m_SpriteSize * 1.25f, m_SpriteSize * 0.5f));
	m_DismissShape.setFillColor(sf::Color::Transparent);
	m_DismissShape.setOutlineThickness(m_OutlineThickness * 0.5f);
	m_DismissShape.setOutlineColor(m_DismissColor);
	m_DismissShape.setPosition(sf::Vector2f(m_DismissPosition));
	m_DismissText.setFont(m_Font);
	m_DismissText.setCharacterSize((int)(m_CharacterSize * 0.5f));
	m_DismissText.setFillColor(m_DismissColor);
	m_DismissText.setString(m_DismissString);
	m_DismissText.setPosition(sf::Vector2f(m_DismissPosition));

	m_MessageTextPosition = sf::Vector2f(m_PositionX + m_SpriteSize * 0.5f, m_PositionY + m_SpriteSize * 1.6f);
	m_MessageText.setFont(m_Font);
	m_MessageText.setCharacterSize((int)(m_CharacterSize * 0.5f));
	m_MessageText.setFillColor(m_OwnerColor);
	m_MessageText.setPosition(sf::Vector2f(m_MessageTextPosition));

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
	m_Window->draw(m_SubjectShape);
	m_Window->draw(m_MessageText);
	if (m_MessageType == UIType::MarriageRequest || m_MessageType == UIType::AllianceRequest || m_MessageType == UIType::PeaceRequest || m_MessageType == UIType::CallToArmsRequest)
	{
		m_Window->draw(m_AgreeShape);
		m_Window->draw(m_AgreeText);
	}
	m_Window->draw(m_DismissShape);
	m_Window->draw(m_DismissText);
	updateSprites();
}

void EventWindow::moveWindow()
{
	Vector2D mousePosition = InputHandler::getUIMousePosition();
	Vector2D diff = m_MousePosition - mousePosition;
	m_PositionX -= diff.x;
	m_PositionY -= diff.y;
	m_MousePosition = mousePosition;
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
	Character& instigator = CharacterManager::get()->getCharacter(m_InstigatorID);
	Character& subject = CharacterManager::get()->getCharacter(m_SubjectID);
	switch (m_MessageType)
	{
		case UIType::MarriageRequest:
		{
			if (!instigator.m_Dead && instigator.m_Spouse == INVALID_CHARACTER_ID
				&& !subject.m_Dead && subject.m_Spouse == INVALID_CHARACTER_ID)
			{
				CharacterManager::get()->onMarriage(m_InstigatorID, m_SubjectID);
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
			CharacterManager::get()->onWarEnded(m_InstigatorID, m_SubjectID);
			break;
		}
		case UIType::CallToArmsRequest:
		{
			std::vector<War> wars = WarManager::get().getWarsOfCharacter(m_InstigatorID);
			Character& playerCharacter = CharacterManager::get()->getPlayerCharacter();
			for (War& war : wars)
			{
				playerCharacter.m_CurrentWars.push_back(war.getHandle());
				UIManager::get()->createWarIcon(war.getAttacker(), war.getDefender());
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
			m_MovingWindow = true;
		}
	}
	if (InputHandler::getLeftMouseReleased())
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		if (m_DismissShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			dismissRequest();
			closeWindow();
		}
		else if (m_AgreeShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			acceptRequest();
			closeWindow();
		}
		else if (m_MovingWindow)
		{
			m_MovingWindow = false;
		}
	}
	else if (InputHandler::getRightMouseReleased())
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		if (m_InstigatorShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			UIManager::get()->m_CharacterWindow->m_CurrentCharacterID = m_InstigatorID;
			UIManager::get()->m_CharacterWindow->checkIfPlayerCharacter();
			UIManager::get()->m_CharacterWindow->updateInfo();
			UIManager::get()->m_CharacterWindow->openWindow();
		}
		else if (m_SubjectShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			UIManager::get()->m_CharacterWindow->m_CurrentCharacterID = m_SubjectID;
			UIManager::get()->m_CharacterWindow->checkIfPlayerCharacter();
			UIManager::get()->m_CharacterWindow->updateInfo();
			UIManager::get()->m_CharacterWindow->openWindow();
		}
	}
}

void EventWindow::updateSprites()
{
	updateSprite(m_InstigatorSprite, m_InstigatorTexture, m_InstigatorPosition);
	updateSprite(m_SubjectSprite, m_SubjectTexture, m_SubjectPosition);
	updateSprite(m_MessageTypeSprite, m_MessageTypeTexture, m_MessageTypePosition, m_SpriteSize / 2);
}

void EventWindow::updateSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, int spriteSize)
{
	sprite.setTexture(texture, true);
	sprite.setPosition(position);

	sf::FloatRect localSize = sprite.getLocalBounds();

	sprite.setScale(spriteSize / localSize.width, spriteSize / localSize.height);

	m_Window->draw(sprite);
}