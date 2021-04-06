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

#include "Game/Systems/Characters/CharacterNamePool.h"

sf::Sound Game::m_Sound;

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
	addEntitys();
}

void Game::run()
{
	sf::RenderWindow* internalWindow = m_Window->getWindow();
	sf::Vector2f floatResolution = sf::Vector2f((float)m_Resolution.x, (float)m_Resolution.y);
	sf::View view(floatResolution * 0.5f, floatResolution);
	internalWindow->setView(view);

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
		UIManager::get()->render();

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
	m_Sound = m_AssetHandler->loadAudioFile("Assets/Audio/Minstrel_Dance.wav", m_SoundBuffer);
	m_Sound.setLoop(true);
	m_Sound.setVolume(volume);
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
	CharacterID char0 = createCharacter(id0, Title::King, "Kingdom of Milano", CharacterNamePool::getMaleName(), 50, 5, false, sf::Color(181, 54, 107));
	UIManager::get()->createUITextElement(m_UIFont, CharacterManager::get()->getCharacter(char0).m_KingdomName, id0);

	std::vector<unsigned int> id1{ 8, 9, 10, 11, 12 };
	CharacterID char1 = createCharacter(id1, Title::Emperor, "Roman Empire", CharacterNamePool::getMaleName(), 100, 10, false, sf::Color(54, 181, 105));
	UIManager::get()->createUITextElement(m_UIFont, CharacterManager::get()->getCharacter(char1).m_KingdomName, id1);

	std::vector<unsigned int> id2{ 13, 14, 15, 16, 17 };
	CharacterID char2 = createCharacter(id2, Title::King, "Kingdom of Sicilies", CharacterNamePool::getMaleName(), 150, 10, true, sf::Color(200, 181, 105));
	UIManager::get()->createUITextElement(m_UIFont, CharacterManager::get()->getCharacter(char2).m_KingdomName, id2);

	//UI
	Vector2D characterWindowPosition = { 10.0f, 10.0f };
	Vector2D characterWindowSize = { 600.0f, 1060.0f };
	UIManager::get()->createUIWindowElement(m_UIFont, UIType::CharacterWindow, characterWindowPosition, characterWindowSize);

	Vector2D regionWindowPosition = { 10.0f, Window::getWindow()->getSize().y - (600.0f + 10.0f) };
	Vector2D regionWindowSize = { 600.0f, 600.0f };
	UIManager::get()->createUIWindowElement(m_UIFont, UIType::RegionWindow, regionWindowPosition, regionWindowSize);

	Vector2D statBarPosition = { Window::getWindow()->getSize().x - (400.0f + 10.0f), 10.0f };
	Vector2D statBarSize = { 400.0f, 50.0f };
	UIManager::get()->createUIWindowElement(m_UIFont, UIType::StatBar, statBarPosition, statBarSize);

	Vector2D dateBarPosition = { Window::getWindow()->getSize().x - (600.0f + 10.0f), Window::getWindow()->getSize().y - (50.0f + 10.0f) };
	Vector2D dateBarSize = { 600.0f, 50.0f };
	UIManager::get()->createUIWindowElement(m_UIFont, UIType::DateBar, dateBarPosition, dateBarSize);
}

CharacterID Game::createCharacter(std::vector<unsigned int>& ownedRegions, Title title, const char* realmName, const char* characterName, int army, int gold, bool playerControlled, sf::Color color)
{
	CharacterID character = CharacterManager::get()->createCharacterWithRandomBirthday(characterName, title, ownedRegions, realmName, army, (float)gold, color, playerControlled, 24, 62);

	for (int i : ownedRegions)
	{
		Map::get().setRegionColor(i, color);
	}

	return character;
}
