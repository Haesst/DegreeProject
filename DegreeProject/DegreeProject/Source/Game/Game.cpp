#include "Game.h"
#include "Engine/Window.h"
#include "Engine/Time.h"
#include "Engine/InputHandler.h"
#include "Game/Systems/UnitManager.h"
#include "Game/HotReloader.h"
#include "Game/GameData.h"
#include "Game/AI/AIManager.h"
#include "Game/UI/UIManager.h"
#include "Game/StaticSpriteManager.h"

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
	sf::Vector2f floatResolution = sf::Vector2f((float)m_Resolution.x, (float)m_Resolution.y);
	sf::View view(floatResolution * 0.5f, floatResolution);
	sf::View uiView(floatResolution * 0.5f, floatResolution);
	internalWindow->setView(view);
	InputHandler::setUIView(uiView);

	//Start
	CharacterManager::get()->start();
	StaticSpriteManager::get()->start();
	UnitManager::get().start();
	AIManager::get().start();
	UIManager::get()->start();

	while (internalWindow->isOpen())
	{
		// Time
		Time::updateTime();

		// Events
		InputHandler::handleInputEvents();

		// Update
		StaticSpriteManager::get()->update();
		UnitManager::get().update();
		CharacterManager::get()->update();
		AIManager::get().update();
		UIManager::get()->update();

		// Render
		Window::getWindow()->clear(sf::Color::Blue);

		// Render map
		Map::get().render();

		CharacterManager::get()->render();
		StaticSpriteManager::get()->render();
		UnitManager::get().render();

		view = Window::getWindow()->getView();
		Window::getWindow()->setView(uiView);
		UIManager::get()->render();
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
	Map::get().setLandTexture(m_AssetHandler->getTextureAtPath("Assets/Graphics/Checkerboard.png"));

	const char* castlePath = "Assets/Graphics/Castle.png";
	for (unsigned int regionIndex = 0; regionIndex < NUMBER_OF_REGIONS; regionIndex++)
	{
		Vector2D castlePosition = Map::convertToScreen(Map::get().m_Data.m_Regions[regionIndex].m_RegionCapital);
		StaticSpriteManager::get()->createStaticSprite(castlePath, 32, 32, castlePosition);
	}

	m_UIFont = m_AssetHandler->loadFontFromFile("Assets/Fonts/TestFont.ttf");

	std::vector<unsigned int> id0{ 1, 2, 3, 4, 5, 6, 7 };
	CharacterID char0 = createCharacter(id0, Title::King, Gender::Male, "Kingdom of Milano", CharacterNamePool::getMaleName(), 50, 5, false, sf::Color(181, 54, 107));
	UIManager::get()->createUITextElement(m_UIFont, char0, CharacterManager::get()->getCharacter(char0).m_KingdomName, id0);
	CharacterManager::get()->addTrait(char0, CharacterManager::get()->getTrait("Ugly"));

	std::vector<unsigned int> id1{ 8, 9, 10, 11, 12 };
	CharacterID char1 = createCharacter(id1, Title::Emperor, Gender::Female, "Roman Empire", CharacterNamePool::getFemaleName(), 100, 10, false, sf::Color(54, 181, 105));
	UIManager::get()->createUITextElement(m_UIFont, char1, CharacterManager::get()->getCharacter(char1).m_KingdomName, id1);

	std::vector<unsigned int> id2{ 13, 14, 15, 16, 17 };
	CharacterID char2 = createCharacter(id2, Title::King, Gender::Male, "Kingdom of Sicilies", CharacterNamePool::getMaleName(), 150, 15, true, sf::Color(200, 181, 105));
	UIManager::get()->createUITextElement(m_UIFont, char2, CharacterManager::get()->getCharacter(char2).m_KingdomName, id2);

	CharacterManager::get()->addTrait(char2, CharacterManager::get()->getTrait("Beautiful"));
	Character& c = CharacterManager::get()->getCharacter(char1);
	Character& c2 = CharacterManager::get()->getCharacter(char0);
	LOG_INFO("{0}'s opinion of Player: {1}", c.m_Name, CharacterManager::get()->getCharacterOpinion(char1, char2));
	LOG_INFO("{0}'s opinion of Player: {1}", c2.m_Name, CharacterManager::get()->getCharacterOpinion(char0, char2));

	std::vector<unsigned int> characterFourRegions{ 18, 20 };
	std::vector<unsigned int> characterFiveRegions{ 19 };
	std::vector<unsigned int> characterSixRegions{ 21 };
	std::vector<unsigned int> characterSevenRegions{ 22 };
	std::vector<unsigned int> characterEightRegions{ 24, 26 };
	std::vector<unsigned int> characterNineRegions{ 25 };
	std::vector<unsigned int> characterTenRegions{ 23 };

	CharacterID char4 = createCharacter(characterFourRegions, Title::Duke, Gender::Male, "Duchy of Krain", CharacterNamePool::getMaleName(), 50, 5, false, sf::Color(16, 181, 191));
	UIManager::get()->createUITextElement(m_UIFont, char4, CharacterManager::get()->getCharacter(char4).m_KingdomName, characterFourRegions);

	CharacterID char5 = createCharacter(characterFiveRegions, Title::Duke, Gender::Male, "Duchy of Slavonia", CharacterNamePool::getMaleName(), 50, 5, false, sf::Color(216, 153, 65));
	UIManager::get()->createUITextElement(m_UIFont, char5, CharacterManager::get()->getCharacter(char5).m_KingdomName, characterFiveRegions);

	CharacterID char6 = createCharacter(characterSixRegions, Title::Duke, Gender::Male, "Duchy of Croatia", CharacterNamePool::getMaleName(), 50, 5, false, sf::Color(37, 130, 198));
	UIManager::get()->createUITextElement(m_UIFont, char6, CharacterManager::get()->getCharacter(char6).m_KingdomName, characterSixRegions);

	CharacterID char7 = createCharacter(characterSevenRegions, Title::Duke, Gender::Male, "Duchy of Lower Bosnia", CharacterNamePool::getMaleName(), 50, 5, false, sf::Color(151, 160, 90));
	UIManager::get()->createUITextElement(m_UIFont, char7, CharacterManager::get()->getCharacter(char7).m_KingdomName, characterSevenRegions);

	CharacterID char8 = createCharacter(characterEightRegions, Title::Duke, Gender::Male, "Duchy of Dubrovnik", CharacterNamePool::getMaleName(), 50, 5, false, sf::Color(240, 140, 149));
	UIManager::get()->createUITextElement(m_UIFont, char8, CharacterManager::get()->getCharacter(char8).m_KingdomName, characterEightRegions);

	CharacterID char9 = createCharacter(characterNineRegions, Title::Duke, Gender::Male, "Duchy of Zachlumia", CharacterNamePool::getMaleName(), 50, 5, false, sf::Color(179, 226, 163));
	UIManager::get()->createUITextElement(m_UIFont, char9, CharacterManager::get()->getCharacter(char9).m_KingdomName, characterNineRegions);

	CharacterID char10 = createCharacter(characterTenRegions, Title::Duke, Gender::Male, "Duchy of Upper Bosnia", CharacterNamePool::getMaleName(), 50, 5, false, sf::Color(24, 225, 191));
	UIManager::get()->createUITextElement(m_UIFont, char10, CharacterManager::get()->getCharacter(char10).m_KingdomName, characterTenRegions);

	//UI
	Vector2D mainMenuPosition = { 960.0f, 540.0f };
	Vector2D mainMenuSize = { 300.0f, 500.0f };
	UIManager::get()->createUIWindowElement(m_UIFont, UIType::MainMenu, mainMenuPosition, mainMenuSize);

	Vector2D characterWindowPosition = { 10.0f, 10.0f };
	Vector2D characterWindowSize = { 600.0f, 1060.0f };
	UIManager::get()->createUIWindowElement(m_UIFont, UIType::CharacterWindow, characterWindowPosition, characterWindowSize);

	Vector2D regionWindowPosition = { 10.0f, Window::getWindow()->getSize().y - (600.0f + 10.0f) };
	Vector2D regionWindowSize = { 600.0f, 600.0f };
	UIManager::get()->createUIWindowElement(m_UIFont, UIType::RegionWindow, regionWindowPosition, regionWindowSize);

	Vector2D warWindowPosition = { 10.0f, Window::getWindow()->getSize().y - (300.0f + 10.0f) };
	Vector2D warWindowSize = { 1900.0f, 300.0f };
	UIManager::get()->createUIWindowElement(m_UIFont, UIType::WarWindow, warWindowPosition, warWindowSize);

	Vector2D statBarPosition = { Window::getWindow()->getSize().x - (400.0f + 10.0f), 10.0f };
	Vector2D statBarSize = { 400.0f, 50.0f };
	UIManager::get()->createUIWindowElement(m_UIFont, UIType::StatBar, statBarPosition, statBarSize);

	Vector2D dateBarPosition = { Window::getWindow()->getSize().x - (600.0f + 10.0f), Window::getWindow()->getSize().y - (50.0f + 10.0f) };
	Vector2D dateBarSize = { 600.0f, 50.0f };
	UIManager::get()->createUIWindowElement(m_UIFont, UIType::DateBar, dateBarPosition, dateBarSize);
}

CharacterID Game::createCharacter(std::vector<unsigned int>& ownedRegions, Title title, Gender gender, const char* realmName, const char* characterName, int army, int gold, bool playerControlled, sf::Color color)
{

	CharacterID character = CharacterManager::get()->createCharacterWithRandomBirthday(characterName, title, gender, ownedRegions, realmName, army, (float)gold, color, playerControlled, 17, 63);

	for (int i : ownedRegions)
	{
		Map::get().setRegionColor(i, color);
	}

	return character;
}
