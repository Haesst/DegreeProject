#include "Game.h"
#include "Engine/Window.h"
#include "Engine/Time.h"
#include "Engine/InputHandler.h"
#include "ECS/EntityManager.h"
#include "Game/Systems/CharacterSystem.h"
#include "Game/HotReloader.h"
#include "Game/AI/AIManager.h"
#include "Game/Components/MovingSprite.h"
#include "Game/Components/SpriteRenderer.h"
#include "Game/Systems/PlayerUnitSystem.h"
#include "Game/Systems/ECSExampleSystem.h"
#include "Game/Systems/SpriteRenderSystem.h"
#include "Game/Systems/UIWindowSystem.h"
#include "Game/Systems/UITextSystem.h"
#include "Game/Systems/UISpriteRenderSystem.h"	
#include "Game/Systems/AI/WarmindSystem.h"
#include "Game/Systems/AI/UnitSystem.h"
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
	entityManager->RegisterSystem<CharacterSystem>();
	entityManager->RegisterSystem<UITextSystem>();
	entityManager->RegisterSystem<ECSExampleSystem>();
	entityManager->RegisterSystem<PlayerUnitSystem>();
	entityManager->RegisterSystem<SpriteRenderSystem>();
	entityManager->RegisterSystem<UIWindowSystem>();
	entityManager->RegisterSystem<UISpriteRenderSystem>();
	entityManager->RegisterSystem<WarmindSystem>();
	entityManager->RegisterSystem<UnitSystem>();
}

void Game::AddEntitys()
{
	EntityManager* entityManager = &EntityManager::Get();

	Map::Init();
	Map::SetLandTexture(m_AssetHandler->GetTextureAtPath("Assets/Graphics/Checkerboard.png"));

	//Create Region Capital Castles ID(00-16)
	const char* castlePath = "Assets/Graphics/Castle.png";
	for (unsigned int regionIndex = 0; regionIndex < m_NumberOfRegions; regionIndex++)
	{
		EntityID castle = entityManager->AddNewEntity();
		entityManager->AddComponent<SpriteRenderer>(castle, castlePath, 32, 32, m_AssetHandler);
		Transform* castleTransform = &entityManager->GetComponent<Transform>(castle);
		castleTransform->m_Position = Map::ConvertToScreen(Map::m_Data.m_Regions[regionIndex].m_RegionCapital);
	}

	//Create PlayerUnit 0 ID(17)
	EntityID playerUnit0 = entityManager->AddNewEntity();
	entityManager->AddComponent<PlayerUnit>(playerUnit0);
	entityManager->AddComponent<SpriteRenderer>(playerUnit0, "Assets/Graphics/Soldier Unit.png", 32, 32, m_AssetHandler);

	//Create Character 0 ID(18)
	EntityID char0 = entityManager->AddNewEntity();
	std::vector<unsigned int> id0{ 0, 1, 2, 3, 4 };
	entityManager->AddComponent<CharacterComponent>(char0, Title::King, "Kingdom of Milano", "Erik", id0, 100, 5, false, sf::Color(181, 54, 107), 0);
	CharacterComponent* characterComponent0 = &entityManager->GetComponent<CharacterComponent>(char0);

	//Create Character 1 ID(19)
	EntityID char1 = entityManager->AddNewEntity();
	std::vector<unsigned int> id1{ 5, 6, 7, 8, 9 };
	entityManager->AddComponent<CharacterComponent>(char1, Title::Emperor, "Roman Republic", "Robin", id1, 100, 10, false, sf::Color(54, 181, 105), 0);
	CharacterComponent* characterComponent1 = &entityManager->GetComponent<CharacterComponent>(char1);

	//Create Character 2 ID(20)
	EntityID char2 = entityManager->AddNewEntity();
	std::vector<unsigned int> id2{ 12, 13, 14, 15, 16 };
	entityManager->AddComponent<CharacterComponent>(char2, Title::Duke, "Sicilian Mobsters", "Fredrik", id2, 100, 10, false, sf::Color(165, 42, 42), 0);
	CharacterComponent* characterComponent2 = &entityManager->GetComponent<CharacterComponent>(char2);

	//Create Character 3 ID(21)
	EntityID char3 = entityManager->AddNewEntity();
	std::vector<unsigned int> id3{ 10 };
	entityManager->AddComponent<CharacterComponent>(char3, Title::Count, "Roman Puppet", "Rob", id3, 100, 10, false, sf::Color(149, 181, 54), 0);
	CharacterComponent* characterComponent3 = &entityManager->GetComponent<CharacterComponent>(char3);

	//Create Character 4 ID(22)
	EntityID char4 = entityManager->AddNewEntity();
	std::vector<unsigned int> id4{ 11 };
	entityManager->AddComponent<CharacterComponent>(char4, Title::Baron, "Sicilian Vassal", "Fred", id4, 100, 10, false, sf::Color(34, 51, 67), 0);
	CharacterComponent* characterComponent4 = &entityManager->GetComponent<CharacterComponent>(char4);

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

	//Create UI window ID(23)
	EntityID windowUI = entityManager->AddNewEntity();
	entityManager->AddComponent<UIWindow>(windowUI, font);

	//Create UI texts ID(24-28)
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
