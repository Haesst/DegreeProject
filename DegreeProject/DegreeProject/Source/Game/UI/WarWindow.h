#pragma once

#include "Game/Data/Types.h"
#include <SFML/Graphics.hpp>
#include "Engine/Vector2D.h"
#include "Game/Data/Titles.h"
#include "Game/GameDate.h"

struct Character;
struct War;
struct Date;
enum class PeaceType;

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
	void updateWarParticipants(CharacterID& mainParticipantID, CharacterID& participantID, std::vector<CharacterID>& participantsIDs, std::vector<sf::RectangleShape>& participantsShapes, std::vector<sf::Sprite>& participantsSprites, std::vector<sf::Texture>& participantsTextures, sf::Vector2f& mainParticipantPosition, sf::Vector2f& participantPosition, unsigned int& index);
	void setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position);
	void setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Vector2f position, const char* string = "");
	void setSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, unsigned int spriteSize = m_SpriteSize);
	void setWarscore(CharacterID& characterID, std::stringstream& stream, int& warscore);
	void onDayChange();
	void updateInfo();
	void handleWindow();
	void clickButton();
	void sendPeaceOffer(PeaceType type);

private:
	sf::RenderWindow* m_Window = nullptr;
	War* m_War = nullptr;
	int m_DaySubscriptionHandle = 0;
	const float m_PortraitScale = 0.125f;
	UIID m_OwnedUIWindow = INVALID_UI_ID;
	float m_SizeX = 1900.0f;
	float m_SizeY = 300.0f;
	float m_PositionX = 0.0f;
	float m_PositionY = 0.0f;
	float m_OutlineThickness = 10.0f;
	int m_CharacterSize = 25;
	const static unsigned int m_SpriteSize = 32;
	bool m_Visible = false;
	sf::Font m_Font;
	sf::Color m_FillColor = sf::Color(255, 252, 240);
	sf::Color m_AttackerColor = sf::Color(210, 32, 60);
	sf::Color m_DefenderColor = sf::Color(33, 137, 206);
	sf::Color m_TextColor = sf::Color::Black;
	sf::Color m_OwnerColor = sf::Color::Black;
	sf::Color m_NegativeColor = sf::Color::Red;
	sf::Color m_PositiveColor = sf::Color::Green;
	sf::Color m_TransparentColor = sf::Color::Transparent;
	Date m_WarStartDate;
	PeaceType m_CurrentPeaceType;

	const char* m_MaleTitles[(unsigned int)Title::Baron + 1] = { "Emperor ", "King ", "Duke ", "Count ", "Baron " };
	const char* m_FemaleTitles[(unsigned int)Title::Baron + 1] = { "Empress ", "Queen ", "Duchess ", "Countess ", "Baroness " };

	sf::Texture m_ArmyTexture;
	sf::Sprite m_AttackerArmySprite;
	sf::Sprite m_DefenderArmySprite;
	sf::Vector2f m_DefenderArmyPosition;
	sf::Vector2f m_AttackerArmyPosition;

	sf::Vector2f m_AttackerPosition;
	sf::Vector2f m_DefenderPosition;
	sf::Vector2f m_WarscorePosition;
	sf::Vector2f m_SurrenderPosition;
	sf::Vector2f m_WhitePeacePosition;
	sf::Vector2f m_EnforceDemandsPosition;
	sf::Vector2f m_SendPosition;

	sf::RectangleShape m_WindowShape;
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

	sf::Vector2f m_WarNameTextPosition;
	sf::Vector2f m_DurationTextPosition;
	sf::Vector2f m_WarscoreTextPosition;
	sf::Vector2f m_WarscoreAmountTextPosition;
	sf::Vector2f m_AttackersTextPosition;
	sf::Vector2f m_AttackerArmyTextPosition;
	sf::Vector2f m_AttackerArmyStrengthTextPosition;
	sf::Vector2f m_DefendersTextPosition;
	sf::Vector2f m_DefenderArmyTextPosition;
	sf::Vector2f m_DefenderArmyStrengthTextPosition;
	sf::Vector2f m_AttackerAlliesTextPosition;
	sf::Vector2f m_DefenderAlliesTextPosition;

	const char* m_Surrender = "Surrender";
	const char* m_WhitePeace = "White Peace";
	const char* m_EnforceDemands = "Enforce Demands";
	const char* m_WarScore = "War Score ";
	const char* m_Allies = "Allies";
	const char* m_TotalSoldiers = "Total Soldiers";
	const char* m_Attackers = "Attackers";
	const char* m_Defenders = "Defenders";
	const char* m_CasusBelli = " war for ";
	const char* m_Of = " of ";
	const char* m_WarStarted = "War started: ";
	const char* m_DaysAgo = " days ago";
	const char* m_MonthsAgo = " months ago";
	const char* m_YearsAgo = " years ago";
	const char* m_DayAgo = " day ago";
	const char* m_MonthAgo = " month ago";
	const char* m_YearAgo = " year ago";
	const char* m_Send = "Send";
	const char* m_PositiveSign = "+";
	const char* m_PercentSign = "%";
	const char* m_DashSign = "/";

	std::vector<CharacterID> m_DefenderCharacterIDs;
	std::vector<sf::RectangleShape> m_DefenderCharacterShapes;
	std::vector<sf::Sprite> m_DefenderCharacterSprites;
	std::vector<sf::Texture> m_DefenderCharacterTextures;
														
	std::vector<CharacterID> m_AttackerCharacterIDs;
	std::vector<sf::RectangleShape> m_AttackerCharacterShapes;
	std::vector<sf::Sprite> m_AttackerCharacterSprites;
	std::vector<sf::Texture> m_AttackerCharacterTextures;
};