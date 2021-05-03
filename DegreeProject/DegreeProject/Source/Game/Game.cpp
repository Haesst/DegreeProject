#include "Game.h"
#include "Engine/Window.h"
#include "Engine/Time.h"
#include "Engine/InputHandler.h"
#include "Game/Systems/UnitManager.h"
#include "Game/HotReloader.h"
#include "Game/GameData.h"
#include "Game/AI/AIManager.h"
#include "Game/UI/UIManager.h"
#include "Game/UI/MiniMap.h"
#include "Game/StaticSpriteManager.h"
#include "Game/DiplomacyManager.h"
#include "Game/Systems/HeraldicShieldManager.h"
#include "Game/Systems/Characters/CharacterNamePool.h"

sf::Sound Game::m_Sound;
sf::Sound Game::m_BattleSound;
sf::Font Game::m_UIFont;

Game::~Game()
{
	delete m_AssetHandler;
}

void Game::init()
{
	std::srand((unsigned int)std::time(nullptr));
	initWindow();
	initHotReloading();
	initAssets();
	HeraldicShieldManager::initialize();
	addEntitys();
}

void Game::run()
{
	sf::RenderWindow* internalWindow = m_Window->getWindow();
	//internalWindow->setFramerateLimit(60);
	sf::Vector2f floatResolution = sf::Vector2f((float)m_Resolution.x, (float)m_Resolution.y);
	sf::View view({ floatResolution.x * 4.5f, floatResolution.y * 1.25f }, floatResolution);
	sf::View uiView(floatResolution * 0.5f, floatResolution);
	internalWindow->setView(view);
	InputHandler::setUIView(uiView);

	//Start
	CharacterManager::get().start();
	StaticSpriteManager::get().start();
	UnitManager::get().start();
	UIManager::get().start();
	DiplomacyManager::get().start();

	UIManager::get().m_MiniMap->setGameView(&view);
	UIManager::get().m_MiniMap->setUIView(&uiView);

	while (internalWindow->isOpen())
	{
		// Time
		Time::updateTime();

		// Events
		InputHandler::handleInputEvents();

		// Update
		StaticSpriteManager::get().update();
		UnitManager::get().update();
		CharacterManager::get().update();
		AIManager::get().update();
		DiplomacyManager::get().update();
		UIManager::get().update();

		// Render
		Window::getWindow()->clear(sf::Color::Blue);

		// Render map
		Map::get().render();

		CharacterManager::get().render();
		StaticSpriteManager::get().render();
		UnitManager::get().render();

		view = Window::getWindow()->getView();
		Window::getWindow()->setView(uiView);
		UIManager::get().render();
		Window::getWindow()->setView(view);

		Window::getWindow()->display();
	}

	m_HotReloader->close();
	m_Window->cleanup();
}

void Game::initWindow()
{
	Window::init(sf::VideoMode(m_Resolution.x, m_Resolution.y), m_GameTitle, sf::Style::Fullscreen);
}

void Game::initHotReloading()
{
	m_HotReloader = HotReloader::get();
}

void Game::initAssets()
{
	m_AssetHandler = new AssetHandler();
	initSound();
	GameData::initialize();
}

void Game::initSound()
{
	m_BattleSound = m_AssetHandler->loadAudioFile("Assets/Audio/battle.wav", m_BattleSoundBuffer);
	m_BattleSound.setLoop(true);
	m_BattleSound.setVolume(m_Volume);

	m_Sound = m_AssetHandler->loadAudioFile("Assets/Audio/Minstrel_Dance.wav", m_SoundBuffer);
	m_Sound.setLoop(true);
	m_Sound.setVolume(m_Volume);
	m_Sound.play();
}

void Game::addEntitys()
{
	Map::get().init();
	Map::get().setLandTexture(m_AssetHandler->getTextureAtPath("Assets/Graphics/NewWorldMap.png"));
	Map::get().setResolution({ (float)m_Resolution.x, (float)m_Resolution.y });

	const char* castlePath = "Assets/Graphics/Castle.png";
	for (unsigned int regionIndex = 0; regionIndex < Map::get().m_Data.m_Regions.size(); regionIndex++)
	{
		Vector2D castlePosition = Map::convertToScreen(Map::get().m_Data.m_Regions[regionIndex].m_RegionCapital);
		StaticSpriteManager::get().createStaticSprite(castlePath, 32, 32, castlePosition);
	}

	m_UIFont = m_AssetHandler->loadFontFromFile("Assets/Fonts/TestFont.ttf");

	std::vector<unsigned int> id0{ 1, 2, 3, 4, 5, 6, 7 };
	CharacterID char0 = createCharacter(id0, Title::King, Gender::Male, "Kingdom of Milano", CharacterNamePool::getMaleName(), 50, 5, false, sf::Color(181, 54, 107));
	UIManager::get().createUITextElement(m_UIFont, char0, CharacterManager::get().getCharacter(char0).m_KingdomName, id0);
	CharacterManager::get().addTrait(char0, CharacterManager::get().getTrait("Ugly"));
	CharacterManager::get().addTrait(char0, CharacterManager::get().getTrait("Sterile"));

	std::vector<unsigned int> id1{ 8, 9, 10, 11, 12 };
	CharacterID char1 = createCharacter(id1, Title::Emperor, Gender::Female, "Roman Empire", CharacterNamePool::getFemaleName(), 100, 10, false, sf::Color(54, 181, 105));
	UIManager::get().createUITextElement(m_UIFont, char1, CharacterManager::get().getCharacter(char1).m_KingdomName, id1);

	std::vector<unsigned int> id2{ 13, 14, 15, 16, 17 };
	CharacterID char2 = createCharacter(id2, Title::King, Gender::Male, "Kingdom of Sicilies", CharacterNamePool::getMaleName(), 150, 15, true, sf::Color(200, 181, 105));
	UIManager::get().createUITextElement(m_UIFont, char2, CharacterManager::get().getCharacter(char2).m_KingdomName, id2);

	CharacterManager::get().addTrait(char2, CharacterManager::get().getTrait("Beautiful"));

	std::vector<unsigned int> characterFourRegions{ 18, 20 };
	std::vector<unsigned int> characterFiveRegions{ 19 };
	std::vector<unsigned int> characterSixRegions{ 21 };
	std::vector<unsigned int> characterSevenRegions{ 22 };
	std::vector<unsigned int> characterEightRegions{ 24, 26 };
	std::vector<unsigned int> characterNineRegions{ 25 };
	std::vector<unsigned int> characterTenRegions{ 23 };
	std::vector<unsigned int> characterElevenRegions{ 27 };
	std::vector<unsigned int> characterTwelveRegions{ 28 };
	std::vector<unsigned int> characterThirteenRegions{ 29 };
	std::vector<unsigned int> characterFourteenRegions{ 30 };

	CharacterID char4 = createCharacter(characterFourRegions, Title::Count, Gender::Male, "County of Krain", CharacterNamePool::getMaleName(), 50, 5, false, sf::Color(16, 181, 191));
	UIManager::get().createUITextElement(m_UIFont, char4, CharacterManager::get().getCharacter(char4).m_KingdomName, characterFourRegions);

	CharacterID char5 = createCharacter(characterFiveRegions, Title::Baron, Gender::Female, "Barony of Slavonia", CharacterNamePool::getFemaleName(), 50, 5, false, sf::Color(216, 153, 65));
	UIManager::get().createUITextElement(m_UIFont, char5, CharacterManager::get().getCharacter(char5).m_KingdomName, characterFiveRegions);

	CharacterID char6 = createCharacter(characterSixRegions, Title::Baron, Gender::Male, "Barony of Croatia", CharacterNamePool::getMaleName(), 50, 5, false, sf::Color(37, 130, 198));
	UIManager::get().createUITextElement(m_UIFont, char6, CharacterManager::get().getCharacter(char6).m_KingdomName, characterSixRegions);

	CharacterID char7 = createCharacter(characterSevenRegions, Title::Baron, Gender::Female, "Barony of Lower Bosnia", CharacterNamePool::getFemaleName(), 50, 5, false, sf::Color(151, 160, 90));
	UIManager::get().createUITextElement(m_UIFont, char7, CharacterManager::get().getCharacter(char7).m_KingdomName, characterSevenRegions);

	CharacterID char8 = createCharacter(characterEightRegions, Title::Count, Gender::Male, "County of Dubrovnik", CharacterNamePool::getMaleName(), 50, 5, false, sf::Color(240, 140, 149));
	UIManager::get().createUITextElement(m_UIFont, char8, CharacterManager::get().getCharacter(char8).m_KingdomName, characterEightRegions);

	CharacterID char9 = createCharacter(characterNineRegions, Title::Baron, Gender::Female, "Barony of Zachlumia", CharacterNamePool::getFemaleName(), 50, 5, false, sf::Color(179, 226, 163));
	UIManager::get().createUITextElement(m_UIFont, char9, CharacterManager::get().getCharacter(char9).m_KingdomName, characterNineRegions);

	CharacterID char10 = createCharacter(characterTenRegions, Title::Baron, Gender::Male, "Barony of Upper Bosnia", CharacterNamePool::getMaleName(), 50, 5, false, sf::Color(24, 225, 191));
	UIManager::get().createUITextElement(m_UIFont, char10, CharacterManager::get().getCharacter(char10).m_KingdomName, characterTenRegions);

	CharacterID char11 = createCharacter(characterElevenRegions, Title::Baron, Gender::Male, "Barony of Corsica", CharacterNamePool::getMaleName(), 50, 5, false, sf::Color(86, 175, 125));
	UIManager::get().createUITextElement(m_UIFont, char11, CharacterManager::get().getCharacter(char11).m_KingdomName, characterElevenRegions);
	
	CharacterID char12 = createCharacter(characterTwelveRegions, Title::Baron, Gender::Female, "Barony of Sardinia", CharacterNamePool::getFemaleName(), 50, 5, false, sf::Color(24, 125, 223));
	UIManager::get().createUITextElement(m_UIFont, char12, CharacterManager::get().getCharacter(char12).m_KingdomName, characterTwelveRegions);

	CharacterID char13 = createCharacter(characterThirteenRegions, Title::Baron, Gender::Female, "Barony of Molise", CharacterNamePool::getFemaleName(), 50, 5, false, sf::Color(165, 189, 72));
	UIManager::get().createUITextElement(m_UIFont, char13, CharacterManager::get().getCharacter(char13).m_KingdomName, characterThirteenRegions);

	CharacterID char14 = createCharacter(characterFourteenRegions, Title::Baron, Gender::Female, "Barony of Valle d'Aosta", CharacterNamePool::getFemaleName(), 50, 5, false, sf::Color(223, 125, 24));
	UIManager::get().createUITextElement(m_UIFont, char14, CharacterManager::get().getCharacter(char14).m_KingdomName, characterFourteenRegions);

	//UI
	Vector2D mainMenuPosition = { 960.0f, 540.0f };
	Vector2D mainMenuSize = { 300.0f, 500.0f };
	UIManager::get().createUIWindowElement(m_UIFont, UIType::MainMenu, mainMenuPosition, mainMenuSize);

	Vector2D endWindowPosition = { 960.0f, 540.0f };
	Vector2D endWindowSize = { 300.0f, 500.0f };
	UIManager::get().createUIWindowElement(m_UIFont, UIType::EndWindow, endWindowPosition, endWindowSize);

	Vector2D pauseWindowPosition = { 960.0f, 270.0f };
	Vector2D pauseWindowSize = { 150.0f, 50.0f };
	UIManager::get().createUIWindowElement(m_UIFont, UIType::PauseWindow, pauseWindowPosition, pauseWindowSize);

	Vector2D characterWindowPosition = { 10.0f, 10.0f };
	Vector2D characterWindowSize = { 600.0f, 1060.0f };
	UIManager::get().createUIWindowElement(m_UIFont, UIType::CharacterWindow, characterWindowPosition, characterWindowSize);

	Vector2D regionWindowPosition = { 10.0f, Window::getWindow()->getSize().y - (600.0f + 10.0f) };
	Vector2D regionWindowSize = { 600.0f, 600.0f };
	UIManager::get().createUIWindowElement(m_UIFont, UIType::RegionWindow, regionWindowPosition, regionWindowSize);

	Vector2D familyTreeWindowPosition = { characterWindowPosition.x * 2 + characterWindowSize.x, 10.0f };
	Vector2D familyTreeWindowSize = { Window::getWindow()->getSize().x - familyTreeWindowPosition.x - 10.0f, 1060.0f };
	UIManager::get().createUIWindowElement(m_UIFont, UIType::FamilyTreeWindow, familyTreeWindowPosition, familyTreeWindowSize);

	Vector2D warWindowPosition = { 10.0f, Window::getWindow()->getSize().y - (300.0f + 10.0f) };
	Vector2D warWindowSize = { 1900.0f, 300.0f };
	UIManager::get().createUIWindowElement(m_UIFont, UIType::WarWindow, warWindowPosition, warWindowSize);

	Vector2D statBarPosition = { Window::getWindow()->getSize().x - (400.0f + 10.0f), 10.0f };
	Vector2D statBarSize = { 400.0f, 50.0f };
	UIManager::get().createUIWindowElement(m_UIFont, UIType::StatBar, statBarPosition, statBarSize);

	Vector2D dateBarPosition = { Window::getWindow()->getSize().x - (600.0f + 10.0f), Window::getWindow()->getSize().y - (50.0f + 10.0f) };
	Vector2D dateBarSize = { 600.0f, 50.0f };
	UIManager::get().createUIWindowElement(m_UIFont, UIType::DateBar, dateBarPosition, dateBarSize);
}

CharacterID Game::createCharacter(std::vector<unsigned int>& ownedRegions, Title title, Gender gender, const char* realmName, const char* characterName, int army, int gold, bool playerControlled, sf::Color color)
{
	CharacterID character = CharacterManager::get().createCharacterWithRandomBirthday(characterName, title, gender, ownedRegions, realmName, army, (float)gold, color, playerControlled, 17, 62);

	for (int i : ownedRegions)
	{
		Map::get().setRegionColor(i, color);
	}

	return character;
}
