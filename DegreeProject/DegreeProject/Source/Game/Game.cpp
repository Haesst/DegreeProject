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
#include <sstream>

sf::Sound Game::m_Sound;
sf::Sound Game::m_BattleSound;
sf::Sound Game::m_VictorySound;
sf::Sound Game::m_DefeatSound;
sf::Font Game::m_UIFont;
sf::View Game::m_GameView;
Window* Game::m_Window;

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
	internalWindow->setVerticalSyncEnabled(true);
	sf::Vector2f floatResolution = sf::Vector2f((float)m_Resolution.x, (float)m_Resolution.y);
	m_GameView.setCenter(m_GameViewCenter);
	m_GameView.setSize(floatResolution);
	sf::View uiView(floatResolution * 0.5f, floatResolution);
	internalWindow->setView(m_GameView);
	InputHandler::setUIView(uiView);

	//Start
	CharacterManager::get().start();
	StaticSpriteManager::get().start();
	UnitManager::get().start();
	UIManager::get().start();
	DiplomacyManager::get().start();

	UIManager::get().m_MiniMap->setGameView(&m_GameView);
	UIManager::get().m_MiniMap->setUIView(&uiView);

	//Logo Splash Screen
	sf::Texture logoTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/UI/Logo.png");
	sf::Sprite logoSprite;
	logoSprite.setTexture(logoTexture, true);
	logoSprite.setScale(1200 / logoSprite.getLocalBounds().width, 700 / logoSprite.getLocalBounds().height);
	logoSprite.setPosition(floatResolution.x * 0.5f - logoSprite.getLocalBounds().width * 0.5f, floatResolution.y * 0.5f - logoSprite.getLocalBounds().height * 0.5f);
	logoSprite.setColor(CharacterManager::get().getPlayerCharacter().m_RegionColor);
	sf::Text logoText;
	logoText.setFont(m_UIFont);
	logoText.setCharacterSize(100);
	logoText.setFillColor(CharacterManager::get().getPlayerCharacter().m_RegionColor);
	std::string realmNameString = CharacterManager::get().getPlayerCharacter().m_KingdomName.substr(6);
	std::stringstream stream;
	stream << CharacterManager::get().getPlayerCharacter().m_Name << realmNameString;
	logoText.setString(stream.str());
	stream.str(std::string());
	stream.clear();
	logoText.setOrigin(logoText.getLocalBounds().width * 0.5f, logoText.getLocalBounds().height * 0.5f);
	logoText.setPosition(floatResolution.x * 0.5f, floatResolution.y * 0.75f);
	while (!InputHandler::m_Inputs[KeyPressed] && !InputHandler::m_Inputs[MouseClicked])
	{
		Window::getWindow()->setView(m_GameView);
		InputHandler::handleInputEvents();
		Window::getWindow()->setView(uiView);
		Window::getWindow()->clear(sf::Color::Black);
		Window::getWindow()->draw(logoSprite);
		Window::getWindow()->draw(logoText);
		Window::getWindow()->display();
	}
	Window::getWindow()->setView(m_GameView);

	Time::pauseGame();
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
		Window::getWindow()->clear(sf::Color::Black);

		// Render map
		Map::get().render();

		CharacterManager::get().render();
		StaticSpriteManager::get().render();
		UnitManager::get().render();

		m_GameView = Window::getWindow()->getView();
		Window::getWindow()->setView(uiView);
		UIManager::get().render();
		Window::getWindow()->setView(m_GameView);

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
	m_BattleSound = m_AssetHandler->loadAudioFile("Assets/Audio/Battle.wav", m_BattleSoundBuffer);
	m_BattleSound.setLoop(true);
	m_BattleSound.setVolume(m_Volume);

	m_VictorySound = m_AssetHandler->loadAudioFile("Assets/Audio/Victory.wav", m_VictorySoundBuffer);
	m_VictorySound.setLoop(true);
	m_VictorySound.setVolume(m_Volume);

	m_DefeatSound = m_AssetHandler->loadAudioFile("Assets/Audio/Defeat.wav", m_DefeatSoundBuffer);
	m_DefeatSound.setLoop(true);
	m_DefeatSound.setVolume(m_Volume);

	m_Sound = m_AssetHandler->loadAudioFile("Assets/Audio/Minstrel_Dance.wav", m_SoundBuffer);
	m_Sound.setLoop(true);
	m_Sound.setVolume(m_Volume);
	m_Sound.play();
}

void Game::setGameViewCenter(sf::Vector2f viewCenter)
{
	m_GameView.setCenter(viewCenter);
	m_Window->getWindow()->setView(m_GameView);
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

	m_PlayerRandomCharacterID = rand() * Map::get().m_Data.m_Regions.size() / RAND_MAX + 1;
	fillEmptyRegionWithOwners();

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

void Game::fillEmptyRegionWithOwners()
{
	bool playerControlled = false;
	CharacterID lastCreatedCharacter = INVALID_CHARACTER_ID;
	for (auto& region : Map::get().m_Data.m_Regions)
	{
		if (region.m_OwnerID == INVALID_CHARACTER_ID)
		{
			playerControlled = (lastCreatedCharacter + 1 == m_PlayerRandomCharacterID) ? true : false;
			if (playerControlled)
			{
				lastCreatedCharacter = addRandomEntityOwningRegion({ region.m_RegionId }, playerControlled, 17, 31);
				Vector2D position = Map::get().convertToScreen(region.m_RegionCapital);
				m_GameViewCenter = { position.x, position.y };
			}
			else
			{
				lastCreatedCharacter = addRandomEntityOwningRegion({ region.m_RegionId });
			}
		}
	}
}

CharacterID Game::addRandomEntityOwningRegion(std::vector<size_t> regions, bool playerControlled, unsigned int minAge, unsigned int maxAge)
{
	bool male = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) <= 0.5f;
	Gender gender = male ? Gender::Male : Gender::Female;
	char* name = male ? CharacterNamePool::getMaleName() : CharacterNamePool::getFemaleName();
	sf::Color regionColor = sf::Color((sf::Uint8)std::rand(), (sf::Uint8)std::rand(), (sf::Uint8)std::rand());
	CharacterID character = CharacterManager::get().createCharacterWithRandomBirthday(name, Title::Baron, gender, regions, "", 10, regionColor, playerControlled, minAge, maxAge);
	UIManager::get().createUITextElement(m_UIFont, character, CharacterManager::get().getCharacter(character).m_KingdomName, regions);
	CharacterManager::get().updateTitleAndUIText(CharacterManager::get().getCharacter(character));
	return character;
}
