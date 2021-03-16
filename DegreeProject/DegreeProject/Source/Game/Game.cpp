#include "Game.h"
#include "Engine/Window.h"
#include "Engine/Time.h"
#include "Engine/InputHandler.h"
#include "Game/MapInfo.h"
#include "ECS/EntityManager.h"
#include "Game/Systems/CharacterSystem.h"
#include "Game/HotReloader.h"
#include "Game/AI/AIManager.h"
#include "Game/Components/MovingSprite.h"
#include "Game/Components/SpriteRenderer.h"
#include "Game/Components/Map.h"
#include "Game/Systems/PlayerUnitSystem.h"
#include "Game/Systems/ECSExampleSystem.h"
#include "Game/Systems/SpriteRenderSystem.h"
#include "Game/Systems/MapSystem.h"
#include "Game/Systems/UIWindowSystem.h"
#include "Game/Systems/UITextSystem.h"
#include "Game/Systems/UISpriteRenderSystem.h"	
#include <Game/Systems/AI/AI.h>

Game::~Game()
{
	delete m_AssetHandler;
}

void Game::Init()
{
	InitWindow();
	InitHotReloading();
	InitAssets();
	InitSystems();
	MapInfo::Initialization(m_NumberOfRegions);
	AddEntitys();
	InitAI();
}

void Game::Run()
{
	sf::RenderWindow* internalWindow = m_Window->GetWindow();
	sf::Vector2f floatResolution = sf::Vector2f((float)m_Resolution.x, (float)m_Resolution.y);
	sf::View view(floatResolution * 0.5f, floatResolution);
	internalWindow->setView(view);

	EntityManager::Get().Start();

	while (internalWindow->isOpen())
	{
		// Time
		Time::UpdateTime();

		// Events
		InputHandler::HandleInputEvents();

		// Update
		EntityManager::Get().Update();
		// Update map
		m_GameDate.Update();

		// Render
		Window::GetWindow()->clear(sf::Color::Blue);
		// Render map
		Map::Render();
		EntityManager::Get().Render();
		Window::GetWindow()->display();
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

void Game::InitSystems()
{
	EntityManager* entityManager = &EntityManager::Get();
	entityManager->RegisterSystem<MapSystem>();
	entityManager->RegisterSystem<CharacterSystem>();
	entityManager->RegisterSystem<UITextSystem>();
	entityManager->RegisterSystem<ECSExampleSystem>();
	entityManager->RegisterSystem<PlayerUnitSystem>();
	entityManager->RegisterSystem<SpriteRenderSystem>();
	entityManager->RegisterSystem<UIWindowSystem>();
	entityManager->RegisterSystem<UISpriteRenderSystem>();
}

void Game::AddEntitys()
{
	EntityManager* entityManager = &EntityManager::Get();

	//Create Map ID(00)
	EntityID map = entityManager->AddNewEntity();
	// entityManager->AddComponent<Map>(map, m_AssetHandler->GetTextureAtPath("Assets/Graphics/Checkerboard.png"));
	// Map* mapComp = entityManager->GetComponentArray<Map>();

	Map::Init();
	Map::SetLandTexture(m_AssetHandler->GetTextureAtPath("Assets/Graphics/Checkerboard.png"));

	//Create Region Capital Castles ID(01-17)
	const char* castlePath = "Assets/Graphics/Castle.png";
	for (unsigned int regionIndex = 0; regionIndex < m_NumberOfRegions; regionIndex++)
	{
		EntityID castle = entityManager->AddNewEntity();
		entityManager->AddComponent<SpriteRenderer>(castle, castlePath, 32, 32, m_AssetHandler);
		Transform* castleTransform = &entityManager->GetComponent<Transform>(castle);
		castleTransform->m_Position = Map::ConvertToScreen(Map::m_Data.m_Regions[regionIndex].m_RegionCapital);
	}

	//Create PlayerUnit 0 ID(18)
	EntityID playerUnit0 = entityManager->AddNewEntity();
	entityManager->AddComponent<PlayerUnit>(playerUnit0);
	entityManager->AddComponent<SpriteRenderer>(playerUnit0, "Assets/Graphics/Soldier Unit.png", 32, 32, m_AssetHandler);

	//Create PlayerUnit 1
	//EntityID playerUnit1 = entityManager->AddNewEntity();
	//entityManager->AddComponent<PlayerUnit>(playerUnit1);
	//entityManager->AddComponent<SpriteRenderer>(playerUnit1, "Assets/Graphics/Soldier Unit.png", 32, 32, m_AssetHandler);

	//Create Character 0 ID(19)
	EntityID char0 = entityManager->AddNewEntity();
	std::vector<unsigned int> id0{ 0, 1, 2, 3, 4 };
	entityManager->AddComponent<CharacterComponent>(char0, Title::King, "Kingdom of Milano", "Erik", id0, 100, 5, false, sf::Color::Red, 0);
	CharacterComponent* characterComponent0 = &entityManager->GetComponent<CharacterComponent>(char0);

	//Create Character 1 ID(20)
	EntityID char1 = entityManager->AddNewEntity();
	std::vector<unsigned int> id1{ 5, 6, 7, 8, 9 };
	entityManager->AddComponent<CharacterComponent>(char1, Title::King, "Roman Republic", "Robin", id1, 100, 10, false, sf::Color::Green, 0);
	CharacterComponent* characterComponent1 = &entityManager->GetComponent<CharacterComponent>(char1);

	//Create Character 2 ID(21)
	EntityID char2 = entityManager->AddNewEntity();
	std::vector<unsigned int> id2{ 12, 13, 14, 15, 16 };
	entityManager->AddComponent<CharacterComponent>(char2, Title::King, "Sicilian Mobsters", "Fredrik", id2, 100, 10, false, sf::Color::Color(165, 42, 42), 0);
	CharacterComponent* characterComponent2 = &entityManager->GetComponent<CharacterComponent>(char2);

	//Create Character 3 ID(22)
	EntityID char3 = entityManager->AddNewEntity();
	std::vector<unsigned int> id3{ 10 };
	entityManager->AddComponent<CharacterComponent>(char3, Title::Baron, "Roman Puppet", "Robin", id3, 100, 10, false, sf::Color::Yellow, 0);
	CharacterComponent* characterComponent3 = &entityManager->GetComponent<CharacterComponent>(char3);

	//Create Character 4 ID(23)
	EntityID char4 = entityManager->AddNewEntity();
	std::vector<unsigned int> id4{ 11 };
	entityManager->AddComponent<CharacterComponent>(char4, Title::Baron, "Sicilian Vassal", "Fredrik", id4, 100, 10, false, sf::Color::Color(128, 128, 128), 0);
	CharacterComponent* characterComponent4 = &entityManager->GetComponent<CharacterComponent>(char4);

	//Create Character Unit 0 ID(24)
	EntityID charUnit0 = entityManager->AddNewEntity();
	entityManager->AddComponent<SpriteRenderer>(charUnit0, "Assets/Graphics/Soldier Unit.png", 32, 32, m_AssetHandler);
	Transform* characterUnitTransform0 = &entityManager->GetComponent<Transform>(charUnit0);
	characterUnitTransform0->m_Position = { m_Window->GetWindow()->getSize().x * 0.65f, m_Window->GetWindow()->getSize().y * 0.25f };
	
	//Create Character Unit 1 ID(25)
	EntityID charUnit1 = entityManager->AddNewEntity();
	entityManager->AddComponent<SpriteRenderer>(charUnit1, "Assets/Graphics/Soldier Unit.png", 32, 32, m_AssetHandler);
	Transform* characterUnitTransform1 = &entityManager->GetComponent<Transform>(charUnit1);
	characterUnitTransform1->m_Position = { m_Window->GetWindow()->getSize().x * 0.65f, m_Window->GetWindow()->getSize().y * 0.35f };

	//Create Character Unit 2 ID(26)
	EntityID charUnit2 = entityManager->AddNewEntity();
	entityManager->AddComponent<SpriteRenderer>(charUnit2, "Assets/Graphics/Soldier Unit.png", 32, 32, m_AssetHandler);
	Transform* characterUnitTransform2 = &entityManager->GetComponent<Transform>(charUnit2);
	characterUnitTransform2->m_Position = { m_Window->GetWindow()->getSize().x * 1.2f, m_Window->GetWindow()->getSize().y * 0.7f };
	
	//Create Character Unit 3 ID(27)
	EntityID charUnit3 = entityManager->AddNewEntity();
	entityManager->AddComponent<SpriteRenderer>(charUnit3, "Assets/Graphics/Soldier Unit.png", 32, 32, m_AssetHandler);
	Transform* characterUnitTransform3 = &entityManager->GetComponent<Transform>(charUnit3);
	characterUnitTransform3->m_Position = { m_Window->GetWindow()->getSize().x * 0.95f, m_Window->GetWindow()->getSize().y * 0.6f };

	//Create Character Unit 4 ID(28)
	EntityID charUnit4 = entityManager->AddNewEntity();
	entityManager->AddComponent<SpriteRenderer>(charUnit4, "Assets/Graphics/Soldier Unit.png", 32, 32, m_AssetHandler);
	Transform* characterUnitTransform4 = &entityManager->GetComponent<Transform>(charUnit4);
	characterUnitTransform4->m_Position = { m_Window->GetWindow()->getSize().x * 1.0f, m_Window->GetWindow()->getSize().y * 0.6f };

	entityManager->AddComponent<WarmindComponent>(char1, 1, char0);
	entityManager->AddComponent<WarmindComponent>(char0, 4, char1);

	CharacterComponent* characters = entityManager->GetComponentArray<CharacterComponent>();
	for (auto region : characters[char0].m_OwnedRegionIDs)
	{
		Map::SetRegionColor(region, characters[char0].m_RegionColor);
	}
	for (auto region : characters[char1].m_OwnedRegionIDs)
	{
		Map::SetRegionColor(region, characters[char1].m_RegionColor);
	}
	for (auto region : characters[char2].m_OwnedRegionIDs)
	{
		Map::SetRegionColor(region, characters[char2].m_RegionColor);
	}
	for (auto region : characters[char3].m_OwnedRegionIDs)
	{
		Map::SetRegionColor(region, characters[char3].m_RegionColor);
	}
	for (auto region : characters[char4].m_OwnedRegionIDs)
	{
		Map::SetRegionColor(region, characters[char4].m_RegionColor);
	}

	sf::Font font = m_AssetHandler->LoadFontFromFile("Assets/Fonts/TestFont.ttf");

	//Create UI window ID(29)
	EntityID windowUI = entityManager->AddNewEntity();
	entityManager->AddComponent<UIWindow>(windowUI, font);

	//Create UI texts ID(30-34)(46)
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
#pragma warning(push)
#pragma warning(disable: 26815)
	AISystem* sys = (AISystem*)entityManager->GetSystem<AISystem>().get();
#pragma warning(pop)

	// Init system with manager component
	sys->Init(m_AIManager);
}
