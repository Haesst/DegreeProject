#include "Game.h"
#include "Engine/Window.h"
#include "Engine/Time.h"
#include "Engine/InputHandler.h"
#include "Engine/UnitManager.h"
#include "Game/HotReloader.h"
#include "Game/GameData.h"
#include "Game/AI/AIManager.h"
#include "Game/UI/UIManager.h"

//Todo: Move from ECS
#include "ECS/EntityManager.h"
#include "Game/Systems/SpriteRenderSystem.h"
#include "Game/Systems/UI/UISpriteRenderSystem.h"

sf::Sound Game::m_Sound;

Game::~Game()
{
	delete m_AssetHandler;
}

void Game::init()
{
	initWindow();
	initHotReloading();
	initAssets();
	UnitManager::get().setAssetHandler(m_AssetHandler);
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
		UnitManager::get().update();
		CharacterManager::get()->update();
		AIManager::get().update();
		UIManager::get()->update();

		// Render
		Window::getWindow()->clear(sf::Color::Blue);

		// Render map
		Map::get().render();

		CharacterManager::get()->render();
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
	EntityManager* entityManager = &EntityManager::get();

	Map::get().init();
	Map::get().setLandTexture(m_AssetHandler->getTextureAtPath("Assets/Graphics/Checkerboard.png"));

	//Todo: Move from ECS
	const char* castlePath = "Assets/Graphics/Castle.png";
	for (unsigned int regionIndex = 0; regionIndex < m_NumberOfRegions; regionIndex++)
	{
		EntityID castle = entityManager->addNewEntity();
		entityManager->addComponent<SpriteRenderer>(castle, castlePath, 32, 32, m_AssetHandler);
		Transform* castleTransform = &entityManager->getComponent<Transform>(castle);
		castleTransform->m_Position = Map::convertToScreen(Map::get().m_Data.m_Regions[regionIndex].m_RegionCapital);
	}

	m_UIFont = m_AssetHandler->loadFontFromFile("Assets/Fonts/TestFont.ttf");

	std::vector<unsigned int> id0{ 1, 2, 3, 4, 5, 6, 7 };
	CharacterID char0 = createCharacter(id0, Title::King, "Kingdom of Milano", "Erik", 50, 5, false, sf::Color(181, 54, 107));
	UIManager::get()->createUITextElement(m_UIFont, CharacterManager::get()->getCharacter(char0).m_KingdomName, id0);

	std::vector<unsigned int> id1{ 8, 9, 10, 11, 12 };
	CharacterID char1 = createCharacter(id1, Title::Emperor, "Roman Empire", "Robin", 100, 10, false, sf::Color(54, 181, 105));
	UIManager::get()->createUITextElement(m_UIFont, CharacterManager::get()->getCharacter(char1).m_KingdomName, id1);

	std::vector<unsigned int> id2{ 13, 14, 15, 16, 17 };
	CharacterID char2 = createCharacter(id2, Title::King, "Kingdom of Sicilies", "Fredrik", 150, 10, true, sf::Color(200, 181, 105));
	UIManager::get()->createUITextElement(m_UIFont, CharacterManager::get()->getCharacter(char2).m_KingdomName, id2);

	//UI Sprite IDs
	//Todo: Move from ECS
	EntityID topPortraitID = entityManager->addNewEntity();
	EntityID bottomPortraitID = entityManager->addNewEntity();
	EntityID regionPortraitID = entityManager->addNewEntity();
	EntityID regionRaiseArmyID = entityManager->addNewEntity();
	EntityID regionBuildSlotID = entityManager->addNewEntity();
	EntityID regionBuildSlotID2 = entityManager->addNewEntity();
	EntityID regionBuildSlotID3 = entityManager->addNewEntity();
	EntityID regionBuildSlotID4 = entityManager->addNewEntity();

	//Todo: Move from ECS
	unsigned int ids[10];
	ids[2] = regionBuildSlotID;
	ids[3] = regionBuildSlotID2;
	ids[4] = regionBuildSlotID3;
	ids[5] = regionBuildSlotID4;
	ids[6] = regionRaiseArmyID;
	ids[7] = topPortraitID;
	ids[8] = bottomPortraitID;
	ids[9] = regionPortraitID;

	//UI
	Vector2D characterWindowPosition = { 10.0f, 10.0f };
	Vector2D characterWindowSize = { 600.0f, 1060.0f };
	ids[0] = UIManager::get()->createUIWindowElement(m_UIFont, UIType::CharacterWindow, characterWindowPosition, characterWindowSize);

	Vector2D regionWindowPosition = { 10.0f, Window::getWindow()->getSize().y - (600.0f + 10.0f) };
	Vector2D regionWindowSize = { 600.0f, 600.0f };
	ids[1] = UIManager::get()->createUIWindowElement(m_UIFont, UIType::RegionWindow, regionWindowPosition, regionWindowSize);

	Vector2D statBarPosition = { Window::getWindow()->getSize().x - (400.0f + 10.0f), 10.0f };
	Vector2D statBarSize = { 400.0f, 50.0f };
	UIManager::get()->createUIWindowElement(m_UIFont, UIType::StatBar, statBarPosition, statBarSize);

	Vector2D dateBarPosition = { Window::getWindow()->getSize().x - (600.0f + 10.0f), Window::getWindow()->getSize().y - (50.0f + 10.0f) };
	Vector2D dateBarSize = { 600.0f, 50.0f };
	UIManager::get()->createUIWindowElement(m_UIFont, UIType::DateBar, dateBarPosition, dateBarSize);

	//Todo: Move from ECS
	EntityID goldSpriteID = entityManager->addNewEntity();
	entityManager->addComponent<UISpriteRenderer>(goldSpriteID, "Assets/Graphics/Coins.png", 32, 32, m_AssetHandler);
	Transform& goldSpriteTransform = entityManager->getComponent<Transform>(goldSpriteID);
	goldSpriteTransform.m_Position = Vector2D(statBarSize.x * 0.1f + m_Resolution.x - statBarSize.x - 10.0f, statBarSize.y * 0.3f);

	EntityID armySpriteID = entityManager->addNewEntity();
	entityManager->addComponent<UISpriteRenderer>(armySpriteID, "Assets/Graphics/soldier unit.png", 32, 32, m_AssetHandler);
	Transform& armySpriteTransform = entityManager->getComponent<Transform>(armySpriteID);
	armySpriteTransform.m_Position = Vector2D(statBarSize.x * 0.5f + m_Resolution.x - statBarSize.x - 10.0f, statBarSize.y * 0.3f);

	entityManager->addComponent<UISpriteRenderer>(topPortraitID, "Assets/Graphics/Unit.png", 64, 64, m_AssetHandler);
	Transform& topPortraitTransform = entityManager->getComponent<Transform>(topPortraitID);
	topPortraitTransform.m_Position = Vector2D(characterWindowSize.x * 0.1f, characterWindowSize.y * 0.025f);
	//entityManager->setEntityActive(topPortraitID, false);

	entityManager->addComponent<UISpriteRenderer>(bottomPortraitID, "Assets/Graphics/Unit.png", 128, 128, m_AssetHandler);
	Transform& bottomPortraitTransform = entityManager->getComponent<Transform>(bottomPortraitID);
	bottomPortraitTransform.m_Position = Vector2D(0.0f, m_Resolution.y - 128.0f);

	entityManager->addComponent<UISpriteRenderer>(regionPortraitID, "Assets/Graphics/Unit.png", 64, 64, m_AssetHandler);
	Transform& regionPortraitTransform = entityManager->getComponent<Transform>(regionPortraitID);
	regionPortraitTransform.m_Position = Vector2D(10.0f + 64, (float)(m_Resolution.y - 64 * 8));
	//entityManager->setEntityActive(regionPortraitID, false);

	float iconSlotPositionX = regionWindowSize.x - 64 - 10.0f;
	float iconSlotPositionOffset = 64 + 10.0f * 2;
	float iconSlotPositionY = m_Resolution.y - 64 - 10.0f * 3;

	entityManager->addComponent<UISpriteRenderer>(regionRaiseArmyID, "Assets/Graphics/soldier unit.png", 64, 64, m_AssetHandler);
	Transform& regionRaiseArmyTransform = entityManager->getComponent<Transform>(regionRaiseArmyID);
	regionRaiseArmyTransform.m_Position = Vector2D(iconSlotPositionX, iconSlotPositionY - iconSlotPositionOffset);
	//entityManager->setEntityActive(regionRaiseArmyID, false);

	entityManager->addComponent<UISpriteRenderer>(regionBuildSlotID, "Assets/Graphics/FortIcon.png", 64, 64, m_AssetHandler);
	Transform& regionBuildSlotTransform = entityManager->getComponent<Transform>(regionBuildSlotID);
	regionBuildSlotTransform.m_Position = Vector2D(iconSlotPositionX, iconSlotPositionY);
	//entityManager->setEntityActive(regionBuildSlotID, false);

	entityManager->addComponent<UISpriteRenderer>(regionBuildSlotID2, "Assets/Graphics/BarackIcon.png", 64, 64, m_AssetHandler);
	Transform& regionBuildSlot2Transform = entityManager->getComponent<Transform>(regionBuildSlotID2);
	regionBuildSlot2Transform.m_Position = Vector2D(iconSlotPositionX - iconSlotPositionOffset, iconSlotPositionY);
	//entityManager->setEntityActive(regionBuildSlotID2, false);

	entityManager->addComponent<UISpriteRenderer>(regionBuildSlotID3, "Assets/Graphics/MarketIcon.png", 64, 64, m_AssetHandler);
	Transform& regionBuildSlot3Transform = entityManager->getComponent<Transform>(regionBuildSlotID3);
	regionBuildSlot3Transform.m_Position = Vector2D(iconSlotPositionX - iconSlotPositionOffset * 2, iconSlotPositionY);
	//entityManager->setEntityActive(regionBuildSlotID3, false);

	entityManager->addComponent<UISpriteRenderer>(regionBuildSlotID4, "Assets/Graphics/WallIcon.png", 64, 64, m_AssetHandler);
	Transform& regionBuildSlot4Transform = entityManager->getComponent<Transform>(regionBuildSlotID4);
	regionBuildSlot4Transform.m_Position = Vector2D(iconSlotPositionX - iconSlotPositionOffset * 3, iconSlotPositionY);
	//entityManager->setEntityActive(regionBuildSlotID4, false);
}

CharacterID Game::createCharacter(std::vector<unsigned int>& ownedRegions, Title title, const char* realmName, const char* characterName, int army, int gold, bool playerControlled, sf::Color color)
{
	CharacterID character = CharacterManager::get()->createCharacter(characterName, title, ownedRegions, realmName, army, (float)gold, color, playerControlled);

	for (int i : ownedRegions)
	{
		Map::get().setRegionColor(i, color);
	}

	return character;
}
