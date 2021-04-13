#pragma once

#include "Game/Data/Types.h"
#include <SFML/Graphics.hpp>
#include "Engine/Vector2D.h"
#include "Game/Data/Titles.h"

struct Character;

class WarWindow
{
public:

	WarWindow(UIID id, sf::Font font, Vector2D, Vector2D size);

	void start();
	void update();
	void render();
	void openWindow();
	void closeWindow();

private:

	void onMonthChange();
	void updateInfo();
	void handleWindow();
	void clickButton();
	void updateSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, int spriteSize = m_SpriteSize);

private:

	sf::RenderWindow* m_Window = nullptr;

	UIID m_OwnedUIWindow = INVALID_UI_ID;
	float m_SizeX = 1900.0f;
	float m_SizeY = 300.0f;
	int m_PositionX = 0;
	int m_PositionY = 0;
	float m_OutlineThickness = 10.0f;
	int m_CharacterSize = 25;
	const static int m_SpriteSize = 32;
	bool m_Open = false;
	bool m_Visible = false;
	sf::Font m_Font;
	sf::Text::Style m_Style = sf::Text::Regular;
	sf::Color m_FillColor = sf::Color(255, 252, 240);
	sf::Color m_AttackerColor = sf::Color(210, 32, 60);
	sf::Color m_DefenderColor = sf::Color(33, 137, 206);
	sf::Color m_TextColor = sf::Color::Black;
	sf::Color m_OwnerColor = sf::Color::Black;

	const std::string m_MaleTitles[(unsigned int)Title::Baron + 1] = { "Emperor ", "King ", "Duke ", "Count ", "Baron " };
	const std::string m_FemaleTitles[(unsigned int)Title::Baron + 1] = { "Empress ", "Queen ", "Duchess ", "Countess ", "Baroness " };
	sf::Texture m_MaleCharacterTexture;
	sf::Texture m_FemaleCharacterTexture;
	sf::Texture m_YoungMaleCharacterTexture;
	sf::Texture m_YoungFemaleCharacterTexture;

	sf::Texture m_ArmyTexture;
	sf::Sprite m_AttackerArmySprite;
	sf::Sprite m_DefenderArmySprite;
	sf::Vector2f m_DefenderArmyPosition = sf::Vector2f();
	sf::Vector2f m_AttackerArmyPosition = sf::Vector2f();

	Vector2D m_AttackerPosition;
	Vector2D m_DefenderPosition;
	Vector2D m_WarscorePosition;
	Vector2D m_WarscoreProgressPosition;
	Vector2D m_SurrenderPosition;
	Vector2D m_WhitePeacePosition;
	Vector2D m_EnforceDemandsPosition;
	Vector2D m_SendPosition;

	sf::RectangleShape m_WindowShape;
	sf::RectangleShape m_AttackerShape;
	sf::RectangleShape m_DefenderShape;
	sf::RectangleShape m_WarscoreShape;
	sf::RectangleShape m_WarscoreProgressShape;
	sf::RectangleShape m_SurrenderShape;
	sf::RectangleShape m_WhitePeaceShape;
	sf::RectangleShape m_EnforceDemandsShape;
	sf::RectangleShape m_SendShape;

	sf::Text m_WarNameText;
	sf::Text m_DurationText;
	sf::Text m_WarscoreText;
	sf::Text m_SurrenderText;
	sf::Text m_WhitePeaceText;
	sf::Text m_EnforceDemandsText;
	sf::Text m_SendText;
	sf::Text m_AttackerArmyText;
	sf::Text m_AttackerArmyStrengthText;
	sf::Text m_DefenderArmyText;
	sf::Text m_DefenderArmyStrengthText;
	sf::Text m_DefenderAlliesText;
	sf::Text m_AttackerAlliesText;

	Vector2D m_WarNameTextPosition;
	Vector2D m_DurationTextPosition;
	Vector2D m_WarscoreTextPosition;
	Vector2D m_SurrenderTextPosition;
	Vector2D m_WhitePeaceTextPosition;
	Vector2D m_EnforceDemandsTextPosition;
	Vector2D m_SendTextPosition;
	Vector2D m_AttackerArmyTextPosition;
	Vector2D m_AttackerArmyStrengthTextPosition;
	Vector2D m_DefenderArmyTextPosition;
	Vector2D m_DefenderArmyStrengthTextPosition;
	Vector2D m_AttackerAlliesTextPosition;
	Vector2D m_DefenderAlliesTextPosition;

	const std::string m_Surrender = "Surrender";
	const std::string m_WhitePeace = "White Peace";
	const std::string m_EnforceDemands = "Enforce Demands";
	const std::string m_WarScore = "War Score ";
	const std::string m_Allies = "Allies";
	const std::string m_TotalSoldiers = "Total Soldiers";
	const std::string m_Attackers = "Attackers";
	const std::string m_Defenders = "Defenders";
	const std::string m_CasusBelli = " war for ";
	const std::string m_Of = " of ";
	const std::string m_WarStarted = "War started: ";
	const std::string m_Ago = " ago";
	const std::string m_Day = " day";
	const std::string m_Month = " month";
	const std::string m_Year = " year";
	const std::string m_Plural = "s";
	const std::string m_Send = "Send";

	std::vector<sf::RectangleShape> m_DefenderCharacterShapes = std::vector<sf::RectangleShape>();
	std::vector<sf::Sprite> m_DefenderCharacterSprites = std::vector<sf::Sprite>();
	std::vector<sf::Texture> m_DefenderCharacterTextures = std::vector<sf::Texture>();
	std::vector<sf::RectangleShape> m_AttackerCharacterShapes = std::vector<sf::RectangleShape>();
	std::vector<sf::Sprite> m_AttackerCharacterSprites = std::vector<sf::Sprite>();
	std::vector<sf::Texture> m_AttackerCharacterTextures = std::vector<sf::Texture>();
	std::vector<sf::Vector2f> m_AttackerCharacterPositions = std::vector<sf::Vector2f>();
	std::vector<sf::Vector2f> m_DefenderCharacterPositions = std::vector<sf::Vector2f>();
};