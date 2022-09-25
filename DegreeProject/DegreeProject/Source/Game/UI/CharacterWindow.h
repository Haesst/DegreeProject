#pragma once

#include <SFML/Graphics.hpp>
#include <Game/Data/Types.h>
#include <Game/Data/Titles.h>
#include <Engine/Vector2D.h>
#include <Game/UI/UIWindow.h>

struct Character;
struct HeraldicShield;
enum class Gender;

enum class Traits
{
	Male,
	Fertility,
	Beautiful,
	Ugly,
	Sterile,
	Spouse,
	Pregnant,
	Female
};

class CharacterWindow : public UIWindow
{
public:

	CharacterWindow(UIID id, sf::Font font, Vector2D, Vector2D size);
	void start() override;
	void update() override;
	void render() override;
	void clickOnMap();
	void onDayChange();
	void clearInfo();
	void updateInfo();
	void updateParents();
	void updateTraits();
	void enableTrait(Traits trait, unsigned int& traitIndex);
	void updateChildren();
	void updateAllies();
	void updateWars();
	void updateTruces();
	void handleWindow();
	void openWindow() override;
	void closeWindow() override;
	void clickButton() override;
	void declareWar();
	void proposeMarriage();
	void proposeAlliance();
	void breakAlliance();
	void assassinate();
	bool checkIfPlayerCharacter();

	std::vector<sf::Text> m_OwnedRegionsTexts = std::vector<sf::Text>();
	std::vector<sf::RectangleShape> m_OwnedRegionShapes = std::vector<sf::RectangleShape>();
	std::vector<HeraldicShield*> m_OwnedRegionShields = std::vector<HeraldicShield*>();
	std::vector<bool> m_OwnedRegionsShowInfo = std::vector<bool>();

	std::vector<sf::RectangleShape> m_ButtonShapes = std::vector<sf::RectangleShape>();
	std::vector<sf::Text> m_ButtonTexts = std::vector<sf::Text>();
	const std::vector<const char*> m_ButtonStrings = { "Declare War", "Marriage", "Alliance", "Break Alliance", "Assassinate" };
	
	std::vector<sf::Text> m_DiplomacyTexts = std::vector<sf::Text>();
	std::vector<sf::Texture> m_DiplomacyTextures = std::vector<sf::Texture>();
	std::vector<sf::Sprite> m_DiplomacySprites = std::vector<sf::Sprite>();
	const std::vector<const char*> m_DiplomacyStrings = { "Regions", "Children", "Alliances", "Conflicts", "Parents", "Truces" };
	std::vector<bool> m_DiplomacyShowInfo = std::vector<bool>();

	std::vector<sf::RectangleShape> m_ChildrenShapes = std::vector<sf::RectangleShape>();
	std::vector<sf::Sprite> m_ChildrenSprites = std::vector<sf::Sprite>();
	std::vector<sf::Texture> m_ChildrenTextures = std::vector<sf::Texture>();

	std::vector<sf::RectangleShape> m_AlliesShapes = std::vector<sf::RectangleShape>();
	std::vector<sf::Sprite> m_AlliesSprites = std::vector<sf::Sprite>();
	std::vector<Vector2DInt> m_AlliesPositions = std::vector<Vector2DInt>();
	std::vector<CharacterID> m_AlliesIDs = std::vector<CharacterID>();
	std::vector<sf::Texture> m_AlliesTextures = std::vector<sf::Texture>();

	std::vector<sf::RectangleShape> m_WarShapes = std::vector<sf::RectangleShape>();
	std::vector<sf::Sprite> m_WarSprites = std::vector<sf::Sprite>();
	std::vector<Vector2DInt> m_WarPositions = std::vector<Vector2DInt>();
	std::vector<CharacterID> m_WarDefenders = std::vector<CharacterID>();
	std::vector<CharacterID> m_WarAttackers = std::vector<CharacterID>();
	std::vector<sf::Texture> m_WarTextures = std::vector<sf::Texture>();

	std::vector<sf::RectangleShape> m_TruceShapes = std::vector<sf::RectangleShape>();
	std::vector<sf::Sprite> m_TruceSprites = std::vector<sf::Sprite>();
	std::vector<sf::Text> m_TruceTexts = std::vector<sf::Text>();
	std::vector<CharacterID> m_TruceIDs = std::vector<CharacterID>();
	std::vector<sf::Texture> m_TruceTextures = std::vector<sf::Texture>();

	const unsigned int m_NumberOfTraits = 8;
	std::vector<sf::Texture> m_TraitTextures = std::vector<sf::Texture>();
	std::vector<sf::Sprite> m_TraitSprites = std::vector<sf::Sprite>();
	std::vector<sf::Text> m_TraitTexts = std::vector<sf::Text>();
	std::vector<sf::Vector2f> m_TraitPositions = std::vector<sf::Vector2f>();
	std::vector<bool> m_TraitsShowText = std::vector<bool>();
	std::vector<bool> m_TraitsEnabled = std::vector<bool>();
	std::vector<const char*> m_TraitStrings = { "Male", "Fertility: ", "Beautiful", "Ugly", "Sterile", "Spouse: ", "Pregnant", "Female" };

	sf::Texture m_MaleChildTexture;
	sf::Texture m_FemaleChildTexture;
	sf::Color m_OwnerColor = sf::Color::Red;
	sf::Color m_DeclareWarColor = sf::Color(210, 32, 60);
	sf::Color m_MakePeaceColor = sf::Color(67, 175, 17);

	sf::Text m_RealmNameText;
	sf::Text m_GoldText;
	sf::Text m_ArmyText;
	sf::Text m_CharacterNameText;
	sf::Text m_CharacterAgeText;
	std::vector<sf::Text> m_ChildrenNames = std::vector<sf::Text>();
	std::vector<bool> m_ShowChildrenNames = std::vector<bool>();
	unsigned int m_CurrentRegionID = 0;
	CharacterID m_CurrentCharacterID = INVALID_CHARACTER_ID;
	const char* m_Dash = "/";
	const char* m_EmptyString = "";
	std::vector<const char*> m_MaleTitles = { "Emperor ", "King ", "Duke ", "Count ", "Baron " };
	std::vector<const char*> m_FemaleTitles = { "Empress ", "Queen ", "Duchess ", "Countess ", "Baroness " };
	int m_DaySubscriptionHandle = -1;
	bool m_Open = false;
	const float m_AssassinationCost = 50.0f;
	const float m_AssassinationChance = 0.5f;
	Character* m_PlayerCharacter = nullptr;
	Character* m_CurrentCharacter = nullptr;
	bool m_IsPlayerCharacter = false;

	unsigned int m_CurrentWars = 0;

	CharacterID m_PreviousCharacterID = INVALID_CHARACTER_ID;
	sf::Texture m_CurrentCharacterTexture;
	sf::Sprite m_CharacterSprite;
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

	sf::Texture m_PregnantTexture;
	sf::Sprite m_PregnantSprite;
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
	sf::Vector2f m_BirthPosition;
	sf::Vector2f m_DeadPosition;
	bool m_Dead = false;
	sf::Sprite m_BirthSprite;
	sf::Text m_DeathDate;
	sf::Text m_Birthday;

	sf::Sprite m_FamilyTreeSprite;
	sf::Texture m_FamilyTreeTexture;
	sf::Text m_FamilyTreeText;
	bool m_ShowFamilyInfo = false;
};