#include "Game.h"

#include "Engine/Window.h"
#include "Engine/Time.h"
#include "Engine/InputHandler.h"
#include "Game/MapInfo.h"
#include "ECS/EntityManager.h"
#include "Game/Systems/CharacterSystem.h"
#include "Game/MapDrawer.h"
#include "Game/HotReloader.h"
#include "Game/AI/AIManager.h"
#include "Game/Components/MovingSprite.h"
#include "Game/Components/SpriteRenderer.h"
#include "Game/Components/Map.h"
#include "Game/Systems/PlayerSystem.h"
#include "Game/Systems/ECSExampleSystem.h"
#include "Game/Systems/SpriteRenderSystem.h"
#include "Game/Systems/MapSystem.h"
#include "Game/Systems/UIWindowSystem.h"
#include "Game/Systems/UITextSystem.h"
#include <Game/Systems/AI/AI.h>

Game::~Game()
{
	delete m_MapDrawer;
	delete m_AssetHandler;
}

void Game::Init()
{
	InitWindow();
	InitHotReloading();
	InitAssets();
	InitMap();
	InitSystems();
	MapInfo::Initialization(17);
	AddEntitys();
	InitAI();
}

void Game::Run()
{
	sf::RenderWindow* internalWindow = m_Window->GetWindow();
	sf::View view(sf::Vector2f(m_Resolution.x * 0.5f, m_Resolution.y * 0.5f), sf::Vector2f(m_Resolution.x, m_Resolution.y));
	internalWindow->setView(view);

	while (internalWindow->isOpen())
	{
		// Time
		Time::UpdateTime();

		// Events
		InputHandler::HandleInputEvents();

		// Update
		EntityManager::Get().Update();

		// Render
		EntityManager::Get().Render();
	}

	m_HotReloader->Close();
	m_Window->Cleanup();
}

void Game::InitWindow()
{
	Window::Init(sf::VideoMode(m_Resolution.x, m_Resolution.y), m_GameTitle, sf::Style::Fullscreen);
}

void Game::InitHotReloading()
{
	m_HotReloader = HotReloader::Get();
}

void Game::InitAssets()
{
	m_AssetHandler = new AssetHandler();
	InitSound();
}

void Game::InitSound()
{
	m_Sound = m_AssetHandler->LoadAudioFile("Assets/Audio/MenuMusic.wav", m_SoundBuffer);
	m_Sound.setVolume(0);
	m_Sound.play();
}

void Game::InitMap()
{
	m_MapDrawer = new MapDrawer();
	m_Map = m_MapDrawer->GetMap("Assets/Map/Map.txt", 'O', { m_Resolution.x, m_Resolution.y }, 30.0f);
}

void Game::InitSystems()
{
	EntityManager* entityManager = &EntityManager::Get();
	entityManager->RegisterSystem<MapSystem>();
	entityManager->RegisterSystem<ECSExampleSystem>();
	entityManager->RegisterSystem<SpriteRenderSystem>();
	entityManager->RegisterSystem<PlayerSystem>();
	entityManager->RegisterSystem<UIWindowSystem>();
	entityManager->RegisterSystem<UITextSystem>();
}

void Game::AddEntitys()
{
	EntityManager* entityManager = &EntityManager::Get();

	//Create map
	EntityID map = entityManager->AddNewEntity();
	entityManager->AddComponent<Map>(map, m_AssetHandler->GetTextureAtPath("Assets/Graphics/TileSet.png"));
	Map* mapComp = entityManager->GetComponentArray<Map>();

	// Create an entity
	EntityID dot1 = entityManager->AddNewEntity();
	// Add necessary components
	entityManager->AddComponent<MovingSprite>(dot1);
	entityManager->AddComponent<SpriteRenderer>(dot1, "Assets/Graphics/Charizard.png", 32, 32, m_AssetHandler);

	// Get transform of entity (or any other component)
	Transform* dot1Transform = &entityManager->GetComponent<Transform>(dot1);
	dot1Transform->m_Position = { m_Window->GetWindow()->getSize().x * 0.5f, m_Window->GetWindow()->getSize().y * 0.5f };

	// Create another entity
	EntityID dot2 = entityManager->AddNewEntity();
	entityManager->AddComponent<SpriteRenderer>(dot2, "Assets/Graphics/Test.jpg", 32, 32, m_AssetHandler);
	// Add necessary components
	entityManager->AddComponent<MovingSprite>(dot2);

	//Create Player Dot
	EntityID playerDot = entityManager->AddNewEntity();
	entityManager->AddComponent<Player>(playerDot);
	Transform* playerDotTransform = &entityManager->GetComponent<Transform>(playerDot);
	Player* playerDotCircle = &entityManager->GetComponent<Player>(playerDot);
	playerDotTransform->m_Position = { m_Window->GetWindow()->getSize().x * 0.5f, m_Window->GetWindow()->getSize().y * 0.6f };

	//Create A Character
	EntityID character = entityManager->AddNewEntity();
	std::vector<int> id{ 0, 1, 2, 3};
	entityManager->AddComponent<CharacterComponent>(character, Title::King, "Italia", "Mussolini", id, 100, 5, false, sf::Color::Red, 0);
	Transform* characterTransform = &entityManager->GetComponent<Transform>(character);

	characterTransform->m_Position = { m_Window->GetWindow()->getSize().x * 0.6f, m_Window->GetWindow()->getSize().y * 0.4f };
	CharacterComponent* characterComponent = &entityManager->GetComponent<CharacterComponent>(character);
	entityManager->AddComponent<SpriteRenderer>(character, "Assets/Graphics/Soldier Unit.png", 32, 32, m_AssetHandler);
	CharacterComponent* characters = entityManager->GetComponentArray<CharacterComponent>();

	EntityID Char2 = entityManager->AddNewEntity();
	std::vector<int> id2{ 4, 5, 6, 7, 8, 9 };
	entityManager->AddComponent<CharacterComponent>(Char2, Title::Count, "Hello", "asdfasdf", id2, 100, 10, false, sf::Color::Green, 0);
	entityManager->AddComponent<WarmindComponent>(Char2, 1, character);

	entityManager->AddComponent<WarmindComponent>(character, 4, Char2);


	for (auto region : characters[character].m_OwnedRegionIDs)
	{
		mapComp[map].SetRegionColor(region, characters[character].m_RegionColor);
	}

	//Create UI window
	EntityID windowUI = entityManager->AddNewEntity();
	entityManager->AddComponent<UIWindow>(windowUI, m_AssetHandler->LoadFontFromFileToText("Assets/Fonts/TestFont.ttf"));

	//Create UI Text
	EntityID textUI = entityManager->AddNewEntity();
	entityManager->AddComponent<UIText>(textUI, m_AssetHandler->LoadFontFromFileToText("Assets/Fonts/TestFont.ttf"), "Kingdom of Bonito", 1150.0f, 475.0f);

	//Create second UI Text
	EntityID textUI2 = entityManager->AddNewEntity();
	entityManager->AddComponent<UIText>(textUI2, m_AssetHandler->LoadFontFromFileToText("Assets/Fonts/TestFont.ttf"), "Republic of Malo", 700.0f, 250.0f, 50, 30.0f, sf::Color::Green, sf::Color::Black, 1.0f, sf::Text::Italic);

	// Get transform and moving circle of entity
	Transform* dot2Transform = &entityManager->GetComponent<Transform>(dot2);
	MovingSprite* dot2Circle = &entityManager->GetComponent<MovingSprite>(dot2);
	dot2Transform->m_Position = { m_Window->GetWindow()->getSize().x * 0.5f, m_Window->GetWindow()->getSize().y * 0.6f };
	dot2Circle->m_Direction = { 0.0f, 1.0f };
	dot2Circle->m_Color = sf::Color::Yellow;
}

void Game::InitAI()
{
	EntityManager* entityManager = &EntityManager::Get();

	// Create entity with AIManagerComoponent
	/*EntityID managerEntity = entityManager->AddNewEntity();
	entityManager->AddComponent<AIManagerComponent>(managerEntity);*/
	m_AIManager = new AIManager();

	// Register system
	entityManager->RegisterSystem<AISystem>();
	// Get system
	AISystem* sys = (AISystem*)entityManager->GetSystem<AISystem>().get();

	// Init system with manager component
	sys->Init(m_AIManager);
}
