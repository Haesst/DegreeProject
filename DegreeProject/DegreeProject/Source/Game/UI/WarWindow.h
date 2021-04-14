#pragma once

#include "Game/Data/Types.h"
#include <SFML/Graphics.hpp>
#include "Engine/Vector2D.h"
#include "Game/Data/Titles.h"
#include "Game/GameDate.h"

struct Character;
struct War;
struct Date;

class WarWindow
{
public:
	WarWindow(UIID id, sf::Font font, Vector2D, Vector2D size);

	void start();
	void update();
	void render();
	void openWindow(CharacterID attackerID, CharacterID defenderID, Date startDate);
	void closeWindow();

private:
	void onDayChange();
	void updateInfo();
	void handleWindow();
	void clickButton();
	void updateSprite(sf::Sprite& sprite, sf::Texture& texture, Vector2DInt position, unsigned int spriteSize = m_SpriteSize);

private:
	sf::RenderWindow* m_Window = nullptr;
	War* m_War = nullptr;
	int m_DaySubscriptionHandle = 0;

	UIID m_OwnedUIWindow = INVALID_UI_ID;
	float m_SizeX = 1900.0f;
	float m_SizeY = 300.0f;
	int m_PositionX = 0;
	int m_PositionY = 0;
	float m_OutlineThickness = 10.0f;
	int m_CharacterSize = 25;
	const static unsigned int m_SpriteSize = 32;
	bool m_Visible = false;
	sf::Font m_Font;
	sf::Text::Style m_Style = sf::Text::Regular;
	sf::Color m_FillColor = sf::Color(255, 252, 240);
	sf::Color m_AttackerColor = sf::Color(210, 32, 60);
	sf::Color m_DefenderColor = sf::Color(33, 137, 206);
	sf::Color m_TextColor = sf::Color::Black;
	sf::Color m_OwnerColor = sf::Color::Black;
	sf::Color m_NegativeColor = sf::Color::Red;
	sf::Color m_PositiveColor = sf::Color::Green;
	Date m_WarStartDate;

	const std::string m_MaleTitles[(unsigned int)Title::Baron + 1] = { "Emperor ", "King ", "Duke ", "Count ", "Baron " };
	const std::string m_FemaleTitles[(unsigned int)Title::Baron + 1] = { "Empress ", "Queen ", "Duchess ", "Countess ", "Baroness " };
	sf::Texture m_MaleCharacterTexture;
	sf::Texture m_FemaleCharacterTexture;
	sf::Texture m_YoungMaleCharacterTexture;
	sf::Texture m_YoungFemaleCharacterTexture;
	sf::Sprite m_AttackerSprite;
	sf::Sprite m_DefenderSprite;
	Vector2DInt m_DefenderPortraitPosition;
	Vector2DInt m_AttackerPortraitPosition;

	sf::Texture m_ArmyTexture;
	sf::Sprite m_AttackerArmySprite;
	sf::Sprite m_DefenderArmySprite;
	Vector2DInt m_DefenderArmyPosition;
	Vector2DInt m_AttackerArmyPosition;

	Vector2DInt m_AttackerPosition;
	Vector2DInt m_DefenderPosition;
	Vector2DInt m_WarscorePosition;
	Vector2DInt m_WarscoreProgressPosition;
	Vector2DInt m_SurrenderPosition;
	Vector2DInt m_WhitePeacePosition;
	Vector2DInt m_EnforceDemandsPosition;
	Vector2DInt m_SendPosition;

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
	sf::Text m_WarscoreAmountText;
	sf::Text m_SurrenderText;
	sf::Text m_WhitePeaceText;
	sf::Text m_EnforceDemandsText;
	sf::Text m_SendText;
	sf::Text m_AttackersText;
	sf::Text m_AttackerArmyText;
	sf::Text m_AttackerArmyStrengthText;
	sf::Text m_DefendersText;
	sf::Text m_DefenderArmyText;
	sf::Text m_DefenderArmyStrengthText;
	sf::Text m_DefenderAlliesText;
	sf::Text m_AttackerAlliesText;

	Vector2DInt m_WarNameTextPosition;
	Vector2DInt m_DurationTextPosition;
	Vector2DInt m_WarscoreTextPosition;
	Vector2DInt m_WarscoreAmountTextPosition;
	Vector2DInt m_SurrenderTextPosition;
	Vector2DInt m_WhitePeaceTextPosition;
	Vector2DInt m_EnforceDemandsTextPosition;
	Vector2DInt m_SendTextPosition;
	Vector2DInt m_AttackersTextPosition;
	Vector2DInt m_AttackerArmyTextPosition;
	Vector2DInt m_AttackerArmyStrengthTextPosition;
	Vector2DInt m_DefendersTextPosition;
	Vector2DInt m_DefenderArmyTextPosition;
	Vector2DInt m_DefenderArmyStrengthTextPosition;
	Vector2DInt m_AttackerAlliesTextPosition;
	Vector2DInt m_DefenderAlliesTextPosition;

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
	const std::string m_DaysAgo = " days ago";
	const std::string m_MonthsAgo = " months ago";
	const std::string m_YearsAgo = " years ago";
	const std::string m_DayAgo = " day ago";
	const std::string m_MonthAgo = " month ago";
	const std::string m_YearAgo = " year ago";
	const std::string m_Send = "Send";
	const std::string m_PositiveSign = "+";
	const std::string m_PercentSign = "%";
	const std::string m_DashSign = "/";

	std::vector<CharacterID> m_DefenderCharacterIDs = std::vector<CharacterID>();
	std::vector<sf::RectangleShape> m_DefenderCharacterShapes = std::vector<sf::RectangleShape>();
	std::vector<sf::Sprite> m_DefenderCharacterSprites = std::vector<sf::Sprite>();
	std::vector<sf::Texture> m_DefenderCharacterTextures = std::vector<sf::Texture>();
	std::vector<Vector2DInt> m_DefenderCharacterPositions = std::vector<Vector2DInt>();

	std::vector<CharacterID> m_AttackerCharacterIDs = std::vector<CharacterID>();
	std::vector<sf::RectangleShape> m_AttackerCharacterShapes = std::vector<sf::RectangleShape>();
	std::vector<sf::Sprite> m_AttackerCharacterSprites = std::vector<sf::Sprite>();
	std::vector<sf::Texture> m_AttackerCharacterTextures = std::vector<sf::Texture>();
	std::vector<Vector2DInt> m_AttackerCharacterPositions = std::vector<Vector2DInt>();
};