#pragma once

#include <SFML/Graphics.hpp>
#include "Game/Data/Types.h"
#include "Game/Data/Titles.h"
#include "Engine/Vector2D.h"

struct Character;
enum class Gender;

#pragma warning(push)
#pragma warning(disable: 26812)
class CharacterWindow
{
public:

	CharacterWindow(UIID id, sf::Font font, Vector2D, Vector2D size);
	void start();
	void update();
	void render();
	void clickOnMap();
	void onDayChange();
	void updateInfo();
	void handleWindow();
	void openWindow();
	void closeWindow();
	void clickButton();
	bool checkIfPlayerCharacter();
	void updateSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, int spriteSize = m_SpriteSize);

	sf::RectangleShape m_WindowShape;
	sf::RectangleShape m_DeclareWarShape;
	sf::RectangleShape m_MakePeaceShape;
	sf::RectangleShape m_MarriageShape;
	sf::RectangleShape m_AllianceShape;
	sf::RectangleShape m_AssassinateShape;
	std::vector<sf::RectangleShape> m_ChildrenShapes = std::vector<sf::RectangleShape>();
	std::vector<sf::Sprite> m_ChildrenSprites = std::vector<sf::Sprite>();
	std::vector<sf::Texture> m_ChildrenTextures = std::vector<sf::Texture>();

	std::vector<sf::RectangleShape> m_AlliesShapes = std::vector<sf::RectangleShape>();
	std::vector<sf::Sprite> m_AlliesSprites = std::vector<sf::Sprite>();
	std::vector<sf::Texture> m_AlliesTextures = std::vector<sf::Texture>();
	std::vector<Vector2DInt> m_AlliesPositions = std::vector<Vector2DInt>();

	std::vector<sf::RectangleShape> m_WarShapes = std::vector<sf::RectangleShape>();
	std::vector<sf::Sprite> m_WarSprites = std::vector<sf::Sprite>();
	std::vector<sf::Texture> m_WarTextures = std::vector<sf::Texture>();
	std::vector<Vector2DInt> m_WarPositions = std::vector<Vector2DInt>();
	std::vector<CharacterID> m_WarDefenders = std::vector<CharacterID>();
	std::vector<CharacterID> m_WarAttackers = std::vector<CharacterID>();

	sf::Texture m_MaleChildTexture;
	sf::Texture m_FemaleChildTexture;
	sf::Color m_FillColor = sf::Color(255, 252, 240);
	sf::Color m_OwnerColor = sf::Color::Red;
	sf::Color m_DeclareWarColor = sf::Color(210, 32, 60);
	sf::Color m_MakePeaceColor = sf::Color(67, 175, 17);
	float m_SizeX = 600.0f;
	float m_SizeY = 1060.0f;
	float m_OutlineThickness = 10.0f;
	bool m_Visible = false;
	sf::Text m_RealmNameText;
	sf::Text m_GoldText;
	sf::Text m_ArmyText;
	sf::Text m_CharacterNameText;
	sf::Text m_CharacterAgeText;
	sf::Font m_Font;
	sf::Text m_DeclareWarText;
	sf::Text m_MakePeaceText;
	sf::Text m_MarriageText;
	sf::Text m_AllianceText;
	sf::Text m_AssassinateText;
	std::vector<sf::Text> m_ChildrenNames = std::vector<sf::Text>();
	std::vector<bool> m_ShowChildrenNames = std::vector<bool>();
	unsigned int m_CurrentRegionID = 0;
	CharacterID m_CurrentCharacterID = INVALID_CHARACTER_ID;
	const std::string m_DeclareWar = "Declare War";
	const std::string m_MakePeace = "Make Peace";
	const std::string m_Marriage = "Marriage";
	const std::string m_Alliance = "Alliance";
	const std::string m_Assassinate = "Assassinate";
	const std::string m_Dash = "/";
	const char* m_PregnantTrait = "Pregnant";
	std::string m_MaleTitles[(unsigned int)Title::Baron + 1];
	std::string m_FemaleTitles[(unsigned int)Title::Baron + 1];
	int m_CharacterSize = 50;
	sf::Text::Style m_Style = sf::Text::Regular;
	int m_DaySubscriptionHandle = 0;
	bool m_Open = false;

	sf::RenderWindow* m_Window = nullptr;
	Character* m_PlayerCharacter = nullptr;
	Character* m_CurrentCharacter = nullptr;
	bool m_IsPlayerCharacter = false;

	UIID m_OwnedUIWindow = INVALID_UI_ID;
	unsigned int m_CurrentWars = 0;

	sf::Texture m_MaleCharacterTexture;
	sf::Texture m_FemaleCharacterTexture;
	sf::Sprite m_CharacterSprite;
	const static int m_SpriteSize = 64;
	sf::Vector2f m_CharacterPosition;

	sf::Texture m_GoldTexture;
	sf::Sprite m_GoldSprite;
	sf::Vector2f m_GoldPosition;

	sf::Texture m_ArmyTexture;
	sf::Sprite m_ArmySprite;
	sf::Vector2f m_ArmyPosition;

	sf::Texture m_AgeTexture;
	sf::Sprite m_AgeSprite;
	sf::Vector2f m_AgePosition;

	sf::Texture m_MaleGenderTexture;
	sf::Texture m_FemaleGenderTexture;
	sf::Sprite m_GenderSprite;
	sf::Vector2f m_GenderPosition;

	sf::Texture m_PregnantTexture;
	sf::Sprite m_Pregnantprite;
	sf::Vector2f m_PregnantPosition;

	sf::Texture m_MarriedTexture;
	sf::Sprite m_MarriedSprite;
	sf::Vector2f m_MarriedPosition;
	sf::Text m_SpouseName;

	sf::RectangleShape m_MotherShape;
	sf::Texture m_MotherTexture;
	sf::Sprite m_MotherSprite;
	sf::Vector2f m_MotherPosition;
	sf::Text m_MotherName;
	bool m_ShowMotherName = false;
	CharacterID m_MotherID = INVALID_CHARACTER_ID;

	sf::RectangleShape m_FatherShape;
	sf::Texture m_FatherTexture;
	sf::Sprite m_FatherSprite;
	sf::Vector2f m_FatherPosition;
	sf::Text m_FatherName;
	bool m_ShowFatherName = false;
	CharacterID m_FatherID = INVALID_CHARACTER_ID;

	Gender m_Gender;
	CharacterID m_SpouseID = INVALID_CHARACTER_ID;
	bool m_ShowSpouseName = false;
	bool m_Pregnant = false;

	sf::Texture m_DeadTexture;
	sf::Sprite m_DeadSprite;
	sf::Vector2f m_DeadPosition;
	bool m_Dead = false;
};
#pragma warning(pop)