#include "Game.h"
#include "Engine/Window.h"
#include "Engine/Time.h"
#include "Engine/InputHandler.h"
#include "ECS/EntityManager.h"
#include "Game/Systems/CharacterSystem.h"
#include "Game/HotReloader.h"
#include "Game/GameData.h"
#include "Game/AI/AIManager.h"
#include "Game/Components/MovingSprite.h"
#include "Game/Components/SpriteRenderer.h"
#include "Game/Systems/SpriteRenderSystem.h"
#include "Game/Systems/CharacterWindowSystem.h"
#include "Game/Systems/UITextSystem.h"
#include "Game/Systems/UISpriteRenderSystem.h"	
#include "Game/Systems/AI/WarmindSystem.h"
#include "Game/Systems/AI/UnitSystem.h"
#include "Game/Systems/PlayerSystem.h"
#include <Game/Systems/AI/AI.h>
#include "Game/Systems/StatBarSystem.h"
#include "Game/Systems/DateBarSystem.h"
#include "Game/Systems/RegionWindowSystem.h"

Game::~Game()
{
	delete m_AssetHandler;
}

void Game::init()
{
	initWindow();
	initHotReloading();
	initAssets();
	initSystems();
	addEntitys();
	initAI();
	Pathfinding::init(Map::m_Data.m_Regions);
}

void Game::run()
{
	sf::RenderWindow* internalWindow = m_Window->getWindow();
	sf::Vector2f floatResolution = sf::Vector2f((float)m_Resolution.x, (float)m_Resolution.y);
	sf::View view(floatResolution * 0.5f, floatResolution);
	internalWindow->setView(view);

	EntityManager::get().start();

	while (internalWindow->isOpen())
	{
		// Time
		Time::updateTime();

		// Events
		InputHandler::handleInputEvents();

		// Update
		EntityManager::get().update();
		// Update map

		// Render
		Window::getWindow()->clear(sf::Color::Blue);
		// Render map
		Map::render();
		EntityManager::get().render();
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
	m_Sound = m_AssetHandler->loadAudioFile("Assets/Audio/MenuMusic.wav", m_SoundBuffer);
	m_Sound.setVolume(1);
	m_Sound.play();
}

void Game::initSystems()
{
	EntityManager* entityManager = &EntityManager::get();
	entityManager->registerSystem<CharacterSystem>();
	entityManager->registerSystem<UITextSystem>();
	entityManager->registerSystem<UnitSystem>();
	entityManager->registerSystem<WarmindSystem>();
	entityManager->registerSystem<SpriteRenderSystem>();
	entityManager->registerSystem<PlayerSystem>();
	entityManager->registerSystem<DateBarSystem>();
	entityManager->registerSystem<StatBarSystem>();
	entityManager->registerSystem<CharacterWindowSystem>();
	entityManager->registerSystem<RegionWindowSystem>();
	entityManager->registerSystem<UISpriteRenderSystem>();
}

void Game::addEntitys()
{
	EntityManager* entityManager = &EntityManager::get();

	Map::init();
	Map::setLandTexture(m_AssetHandler->getTextureAtPath("Assets/Graphics/Checkerboard.png"));

	const char* castlePath = "Assets/Graphics/Castle.png";
	for (unsigned int regionIndex = 0; regionIndex < m_NumberOfRegions; regionIndex++)
	{
		EntityID castle = entityManager->addNewEntity();
		entityManager->addComponent<SpriteRenderer>(castle, castlePath, 32, 32, m_AssetHandler);
		Transform* castleTransform = &entityManager->getComponent<Transform>(castle);
		castleTransform->m_Position = Map::convertToScreen(Map::m_Data.m_Regions[regionIndex].m_RegionCapital);
	}

	m_UIFont = m_AssetHandler->loadFontFromFile("Assets/Fonts/TestFont.ttf");

	std::vector<unsigned int> id0{ 1, 2, 3, 4, 5, 6, 7 };
	EntityID char0 = createCharacter(*entityManager, id0, Title::King, "Kingdom of Milano", "Erik", 50, 5, false, sf::Color(181, 54, 107));

	std::vector<unsigned int> id1{ 8, 9, 10, 11, 12 };
	EntityID char1 = createCharacter(*entityManager, id1, Title::Emperor, "Roman Empire", "Robin", 100, 10, false, sf::Color(54, 181, 105));

	std::vector<unsigned int> id2{ 13, 14, 15, 16, 17 };
	EntityID char2 = createCharacter(*entityManager, id2, Title::King, "Kingdom of Sicilies", "Fredrik", 150, 10, true, sf::Color(200, 181, 105));

	EntityID playerID = entityManager->addNewEntity();
	entityManager->addComponent<Player>(playerID, char2);

	entityManager->addComponent<WarmindComponent>(char1, 1, char0);
	entityManager->addComponent<WarmindComponent>(char0, 4, char1);

	//UI DateBar
	EntityID dateBarID = entityManager->addNewEntity();
	entityManager->addComponent<DateBar>(dateBarID, m_UIFont);

	//UI StatBar
	EntityID statBarID = entityManager->addNewEntity();
	entityManager->addComponent<StatBar>(statBarID, m_UIFont);
	StatBar& statBarComponent = entityManager->getComponent<StatBar>(statBarID);

	EntityID goldSpriteID = entityManager->addNewEntity();
	entityManager->addComponent<UISpriteRenderer>(goldSpriteID, "Assets/Graphics/Coins.png", 32, 32, m_AssetHandler);
	Transform& goldSpriteTransform = entityManager->getComponent<Transform>(goldSpriteID);
	goldSpriteTransform.m_Position = Vector2D(statBarComponent.m_SizeX * 0.1f + m_Resolution.x - statBarComponent.m_SizeX - statBarComponent.m_OutlineThickness, statBarComponent.m_SizeY * 0.3f);

	EntityID armySpriteID = entityManager->addNewEntity();
	entityManager->addComponent<UISpriteRenderer>(armySpriteID, "Assets/Graphics/soldier unit.png", 32, 32, m_AssetHandler);
	Transform& armySpriteTransform = entityManager->getComponent<Transform>(armySpriteID);
	armySpriteTransform.m_Position = Vector2D(statBarComponent.m_SizeX * 0.5f + m_Resolution.x - statBarComponent.m_SizeX - statBarComponent.m_OutlineThickness, statBarComponent.m_SizeY * 0.3f);

	//UI IDs
	EntityID characterWindowID = entityManager->addNewEntity();
	EntityID topPortraitID = entityManager->addNewEntity();
	EntityID bottomPortraitID = entityManager->addNewEntity();
	EntityID regionWindowID = entityManager->addNewEntity();
	EntityID regionPortraitID = entityManager->addNewEntity();
	EntityID regionRaiseArmyID = entityManager->addNewEntity();
	EntityID regionBuildSlotID = entityManager->addNewEntity();
	EntityID regionBuildSlotID2 = entityManager->addNewEntity();
	EntityID regionBuildSlotID3 = entityManager->addNewEntity();
	EntityID regionBuildSlotID4 = entityManager->addNewEntity();

	//UI CharacterWindow
	entityManager->addComponent<CharacterWindow>(characterWindowID, m_UIFont, topPortraitID, regionWindowID, bottomPortraitID);
	CharacterWindow& characterWindowComponent = entityManager->getComponent<CharacterWindow>(characterWindowID);

	entityManager->addComponent<UISpriteRenderer>(topPortraitID, "Assets/Graphics/Unit.png", 64, 64, m_AssetHandler);
	Transform& topPortraitTransform = entityManager->getComponent<Transform>(topPortraitID);
	topPortraitTransform.m_Position = Vector2D(characterWindowComponent.m_SizeX * 0.1f, characterWindowComponent.m_SizeY * 0.025f);
	entityManager->setEntityActive(topPortraitID, false);

	entityManager->addComponent<UISpriteRenderer>(bottomPortraitID, "Assets/Graphics/Unit.png", 128, 128, m_AssetHandler);
	Transform& bottomPortraitTransform = entityManager->getComponent<Transform>(bottomPortraitID);
	bottomPortraitTransform.m_Position = Vector2D(0.0f, m_Resolution.y - 128.0f);

	//UI RegionWindow
	EntityID regionBuildSlotIDs[BUILDING_SLOTS] = { regionBuildSlotID, regionBuildSlotID2, regionBuildSlotID3, regionBuildSlotID4 };
	entityManager->addComponent<RegionWindow>(regionWindowID, m_UIFont, regionPortraitID, regionBuildSlotIDs, regionRaiseArmyID, characterWindowID, bottomPortraitID);
	RegionWindow& regionWindowComponent = entityManager->getComponent<RegionWindow>(regionWindowID);

	entityManager->addComponent<UISpriteRenderer>(regionPortraitID, "Assets/Graphics/Unit.png", 64, 64, m_AssetHandler);
	Transform& regionPortraitTransform = entityManager->getComponent<Transform>(regionPortraitID);
	regionPortraitTransform.m_Position = Vector2D(regionWindowComponent.m_OutlineThickness + 64, (float)(m_Resolution.y - 64 * 8));
	entityManager->setEntityActive(regionPortraitID, false);

	float iconSlotPositionX = regionWindowComponent.m_SizeX - 64 - regionWindowComponent.m_OutlineThickness;
	float iconSlotPositionOffset = 64 + regionWindowComponent.m_OutlineThickness * 2;
	float iconSlotPositionY = m_Resolution.y - 64 - regionWindowComponent.m_OutlineThickness * 3;

	entityManager->addComponent<UISpriteRenderer>(regionRaiseArmyID, "Assets/Graphics/soldier unit.png", 64, 64, m_AssetHandler);
	Transform& regionRaiseArmyTransform = entityManager->getComponent<Transform>(regionRaiseArmyID);
	regionRaiseArmyTransform.m_Position = Vector2D(iconSlotPositionX, iconSlotPositionY - iconSlotPositionOffset);
	entityManager->setEntityActive(regionRaiseArmyID, false);

	entityManager->addComponent<UISpriteRenderer>(regionBuildSlotID, "Assets/Graphics/FortIcon.png", 64, 64, m_AssetHandler);
	Transform& regionBuildSlotTransform = entityManager->getComponent<Transform>(regionBuildSlotID);
	regionBuildSlotTransform.m_Position = Vector2D(iconSlotPositionX, iconSlotPositionY);
	entityManager->setEntityActive(regionBuildSlotID, false);

	entityManager->addComponent<UISpriteRenderer>(regionBuildSlotID2, "Assets/Graphics/BarackIcon.png", 64, 64, m_AssetHandler);
	Transform& regionBuildSlot2Transform = entityManager->getComponent<Transform>(regionBuildSlotID2);
	regionBuildSlot2Transform.m_Position = Vector2D(iconSlotPositionX - iconSlotPositionOffset, iconSlotPositionY);
	entityManager->setEntityActive(regionBuildSlotID2, false);

	entityManager->addComponent<UISpriteRenderer>(regionBuildSlotID3, "Assets/Graphics/MarketIcon.png", 64, 64, m_AssetHandler);
	Transform& regionBuildSlot3Transform = entityManager->getComponent<Transform>(regionBuildSlotID3);
	regionBuildSlot3Transform.m_Position = Vector2D(iconSlotPositionX - iconSlotPositionOffset * 2, iconSlotPositionY);
	entityManager->setEntityActive(regionBuildSlotID3, false);

	entityManager->addComponent<UISpriteRenderer>(regionBuildSlotID4, "Assets/Graphics/WallIcon.png", 64, 64, m_AssetHandler);
	Transform& regionBuildSlot4Transform = entityManager->getComponent<Transform>(regionBuildSlotID4);
	regionBuildSlot4Transform.m_Position = Vector2D(iconSlotPositionX - iconSlotPositionOffset * 3, iconSlotPositionY);
	entityManager->setEntityActive(regionBuildSlotID4, false);
}

void Game::initAI()
{
	EntityManager* entityManager = &EntityManager::get();

	// Create entity with AIManagerComoponent
	/*EntityID managerEntity = entityManager->AddNewEntity();
	entityManager->AddComponent<AIManagerComponent>(managerEntity);*/
	m_AIManager = new AIManager();

	// Register system
	entityManager->registerSystem<AISystem>();
	// Get system
#pragma warning(push)
#pragma warning(disable: 26815)
	AISystem* sys = (AISystem*)entityManager->getSystem<AISystem>().get();
#pragma warning(pop)

	// Init system with manager component
	sys->init(m_AIManager);
}

EntityID Game::createCharacter(EntityManager& entityManager, std::vector<unsigned int>& ownedRegions, Title title, const char* realmName, const char* characterName, int army, int gold, bool playerControlled, sf::Color color)
{
	EntityID character = entityManager.addNewEntity();
	entityManager.addComponent<CharacterComponent>(character, title, realmName, characterName, ownedRegions, gold, army, playerControlled, color, 0); // 0 is personality index (move away from indicies asap)

	for (int i : ownedRegions)
	{
		Map::setRegionColor(i, color);
	}

	EntityID textUI = entityManager.addNewEntity();
	entityManager.addComponent<UIText>(textUI, m_UIFont, realmName, ownedRegions);

	CharacterComponent& characterComp = entityManager.getComponent<CharacterComponent>(character);
	characterComp.m_TextUI = textUI;

	return character;
}
