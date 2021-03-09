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
#include "Game/Systems/UISpriteRenderSystem.h"	
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
	entityManager->RegisterSystem<UITextSystem>();
	entityManager->RegisterSystem<ECSExampleSystem>();
	entityManager->RegisterSystem<PlayerSystem>();
	entityManager->RegisterSystem<SpriteRenderSystem>();
	entityManager->RegisterSystem<UIWindowSystem>();
	entityManager->RegisterSystem<UISpriteRenderSystem>();
}

void Game::AddEntitys()
{
	EntityManager* entityManager = &EntityManager::Get();

	//Create map
	EntityID map = entityManager->AddNewEntity();
	entityManager->AddComponent<Map>(map, m_AssetHandler->GetTextureAtPath("Assets/Graphics/TileSet.png"));
	Map* mapComp = entityManager->GetComponentArray<Map>();

	//// Create an entity
	//EntityID dot1 = entityManager->AddNewEntity();
	//// Add necessary components
	//entityManager->AddComponent<MovingSprite>(dot1);
	//entityManager->AddComponent<SpriteRenderer>(dot1, "Assets/Graphics/Charizard.png", 32, 32, m_AssetHandler);

	//// Get transform of entity (or any other component)
	//Transform* dot1Transform = &entityManager->GetComponent<Transform>(dot1);
	//dot1Transform->m_Position = { m_Window->GetWindow()->getSize().x * 0.5f, m_Window->GetWindow()->getSize().y * 0.5f };

	//// Create another entity
	//EntityID dot2 = entityManager->AddNewEntity();
	//entityManager->AddComponent<SpriteRenderer>(dot2, "Assets/Graphics/Test.jpg", 32, 32, m_AssetHandler);
	//// Add necessary components
	//entityManager->AddComponent<MovingSprite>(dot2);

	//Create Player Dot
	EntityID playerUnit = entityManager->AddNewEntity();
	entityManager->AddComponent<Player>(playerUnit);
	Transform* playerUnitTransform = &entityManager->GetComponent<Transform>(playerUnit);
	playerUnitTransform->m_Position = { m_Window->GetWindow()->getSize().x * 0.5f, m_Window->GetWindow()->getSize().y * 0.5f };
	entityManager->AddComponent<SpriteRenderer>(playerUnit, "Assets/Graphics/Soldier Unit.png", 32, 32, m_AssetHandler);

	//Create Character 0
	EntityID char0 = entityManager->AddNewEntity();
	std::vector<unsigned int> id0{ 0, 1, 2, 3, 4 };
	entityManager->AddComponent<CharacterComponent>(char0, Title::King, "Kingdom of Milano", "Erik", id0, 100, 5, false, sf::Color::Red, 0);

	CharacterComponent* characterComponent0 = &entityManager->GetComponent<CharacterComponent>(char0);
	entityManager->AddComponent<SpriteRenderer>(char0, "Assets/Graphics/Soldier Unit.png", 32, 32, m_AssetHandler);
	Transform* characterTransform0 = &entityManager->GetComponent<Transform>(char0);
	characterTransform0->m_Position = { m_Window->GetWindow()->getSize().x * 0.3f, m_Window->GetWindow()->getSize().y * 0.2f };

	//Create Character 1
	EntityID char1 = entityManager->AddNewEntity();
	std::vector<unsigned int> id1{ 5, 6, 7, 8, 9 };
	entityManager->AddComponent<CharacterComponent>(char1, Title::King, "Roman Republic", "Robin", id1, 100, 10, false, sf::Color::Green, 0);

	CharacterComponent* characterComponent1 = &entityManager->GetComponent<CharacterComponent>(char1);
	entityManager->AddComponent<SpriteRenderer>(char1, "Assets/Graphics/Soldier Unit.png", 32, 32, m_AssetHandler);
	Transform* characterTransform1 = &entityManager->GetComponent<Transform>(char1);
	characterTransform1->m_Position = { m_Window->GetWindow()->getSize().x * 0.4f, m_Window->GetWindow()->getSize().y * 0.3f };

	//Create Character 2
	EntityID char2 = entityManager->AddNewEntity();
	std::vector<unsigned int> id2{ 12, 13, 14, 15, 16 };
	entityManager->AddComponent<CharacterComponent>(char2, Title::King, "Sicilian Mobsters", "Fredrik", id2, 100, 10, false, sf::Color::Color(165, 42, 42), 0);

	CharacterComponent* characterComponent2 = &entityManager->GetComponent<CharacterComponent>(char2);
	entityManager->AddComponent<SpriteRenderer>(char2, "Assets/Graphics/Soldier Unit.png", 32, 32, m_AssetHandler);
	Transform* characterTransform2 = &entityManager->GetComponent<Transform>(char2);
	characterTransform2->m_Position = { m_Window->GetWindow()->getSize().x * 0.725f, m_Window->GetWindow()->getSize().y * 0.5f };

	//Create Character 3
	EntityID char3 = entityManager->AddNewEntity();
	std::vector<unsigned int> id3{ 10 };
	entityManager->AddComponent<CharacterComponent>(char3, Title::Baron, "Roman Puppet", "Robin", id3, 100, 10, false, sf::Color::Yellow, 0);

	CharacterComponent* characterComponent3 = &entityManager->GetComponent<CharacterComponent>(char3);
	entityManager->AddComponent<SpriteRenderer>(char3, "Assets/Graphics/Soldier Unit.png", 32, 32, m_AssetHandler);
	Transform* characterTransform3 = &entityManager->GetComponent<Transform>(char3);
	characterTransform3->m_Position = { m_Window->GetWindow()->getSize().x * 0.5f, m_Window->GetWindow()->getSize().y * 0.45f };

	//Create Character 4
	EntityID char4 = entityManager->AddNewEntity();
	std::vector<unsigned int> id4{ 11 };
	entityManager->AddComponent<CharacterComponent>(char4, Title::Baron, "Sicilian Vassal", "Fredrik", id4, 100, 10, false, sf::Color::Color(128, 128, 128), 0);

	CharacterComponent* characterComponent4 = &entityManager->GetComponent<CharacterComponent>(char4);
	entityManager->AddComponent<SpriteRenderer>(char4, "Assets/Graphics/Soldier Unit.png", 32, 32, m_AssetHandler);
	Transform* characterTransform4 = &entityManager->GetComponent<Transform>(char4);
	characterTransform4->m_Position = { m_Window->GetWindow()->getSize().x * 0.625f, m_Window->GetWindow()->getSize().y * 0.45f };

	entityManager->AddComponent<WarmindComponent>(char1, 1, char0);
	entityManager->AddComponent<WarmindComponent>(char0, 4, char1);

	CharacterComponent* characters = entityManager->GetComponentArray<CharacterComponent>();
	for (auto region : characters[char0].m_OwnedRegionIDs)
	{
		mapComp[map].SetRegionColor(region, characters[char0].m_RegionColor);
	}
	for (auto region : characters[char1].m_OwnedRegionIDs)
	{
		mapComp[map].SetRegionColor(region, characters[char1].m_RegionColor);
	}
	for (auto region : characters[char2].m_OwnedRegionIDs)
	{
		mapComp[map].SetRegionColor(region, characters[char2].m_RegionColor);
	}
	for (auto region : characters[char3].m_OwnedRegionIDs)
	{
		mapComp[map].SetRegionColor(region, characters[char3].m_RegionColor);
	}
	for (auto region : characters[char4].m_OwnedRegionIDs)
	{
		mapComp[map].SetRegionColor(region, characters[char4].m_RegionColor);
	}

	sf::Font font = m_AssetHandler->LoadFontFromFile("Assets/Fonts/TestFont.ttf");

	//Create UI window
	EntityID windowUI = entityManager->AddNewEntity();
	entityManager->AddComponent<UIWindow>(windowUI, font);

	//Create UI texts
	EntityID textUI0 = entityManager->AddNewEntity();
	entityManager->AddComponent<UIText>(textUI0, font, characterComponent0->m_KingdomName, characterComponent0->m_OwnedRegionIDs);

	EntityID textUI1 = entityManager->AddNewEntity();
	entityManager->AddComponent<UIText>(textUI1, font, characterComponent1->m_KingdomName, characterComponent1->m_OwnedRegionIDs);

	EntityID textUI2 = entityManager->AddNewEntity();
	entityManager->AddComponent<UIText>(textUI2, font, characterComponent2->m_KingdomName, characterComponent2->m_OwnedRegionIDs);

	EntityID textUI3 = entityManager->AddNewEntity();
	entityManager->AddComponent<UIText>(textUI3, font, characterComponent3->m_KingdomName, characterComponent3->m_OwnedRegionIDs);

	EntityID textUI4 = entityManager->AddNewEntity();
	entityManager->AddComponent<UIText>(textUI4, font, characterComponent4->m_KingdomName, characterComponent4->m_OwnedRegionIDs);

	//EntityID textUI5 = entityManager->AddNewEntity();
	//entityManager->AddComponent<UIText>(textUI5, font, "Kingdom of Six", std::vector<unsigned int> { 5 });

	//EntityID textUI6 = entityManager->AddNewEntity();
	//entityManager->AddComponent<UIText>(textUI6, font, "Kingdom of Seven", std::vector<unsigned int> { 6 });

	//EntityID textUI7 = entityManager->AddNewEntity();
	//entityManager->AddComponent<UIText>(textUI7, font, "Kingdom of Eight", std::vector<unsigned int> { 7 });

	//EntityID textUI8 = entityManager->AddNewEntity();
	//entityManager->AddComponent<UIText>(textUI8, font, "Kingdom of Nine", std::vector<unsigned int> { 8 });

	//EntityID textUI9 = entityManager->AddNewEntity();
	//entityManager->AddComponent<UIText>(textUI9, font, "Kingdom of Ten", std::vector<unsigned int> { 9 });

	//EntityID textUI10 = entityManager->AddNewEntity();
	//entityManager->AddComponent<UIText>(textUI10, font, "Kingdom of Eleven", std::vector<unsigned int> { 10 });

	//EntityID textUI11 = entityManager->AddNewEntity();
	//entityManager->AddComponent<UIText>(textUI11, font, "Kingdom of Twelve", std::vector<unsigned int> { 11 });

	//EntityID textUI12 = entityManager->AddNewEntity();
	//entityManager->AddComponent<UIText>(textUI12, font, "Kingdom of Thirteen", std::vector<unsigned int> { 12 });

	//EntityID textUI13 = entityManager->AddNewEntity();
	//entityManager->AddComponent<UIText>(textUI13, font, "Republic of Fourteen", std::vector<unsigned int> { 13 });

	//EntityID textUI14 = entityManager->AddNewEntity();
	//entityManager->AddComponent<UIText>(textUI14, font, "Kingdom of Fifteen", std::vector<unsigned int> { 14 });

	//EntityID textUI15 = entityManager->AddNewEntity();
	//entityManager->AddComponent<UIText>(textUI15, font, "Kingdom of Sixteen", std::vector<unsigned int> { 15 });

	//EntityID textUI16 = entityManager->AddNewEntity();
	//entityManager->AddComponent<UIText>(textUI16, font, "Kingdom of Seventeen", std::vector<unsigned int> { 16 });

	// Get transform and moving circle of entity
	//Transform* dot2Transform = &entityManager->GetComponent<Transform>(dot2);
	//MovingSprite* dot2Circle = &entityManager->GetComponent<MovingSprite>(dot2);
	//dot2Transform->m_Position = { m_Window->GetWindow()->getSize().x * 0.5f, m_Window->GetWindow()->getSize().y * 0.6f };
	//dot2Circle->m_Direction = { 0.0f, 1.0f };
	//dot2Circle->m_Color = sf::Color::Yellow;
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
