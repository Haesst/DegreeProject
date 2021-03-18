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

	const char* castlePath = "Assets/Graphics/Castle.png";
	for (unsigned int regionIndex = 0; regionIndex < m_NumberOfRegions; regionIndex++)
	{
		EntityID castle = entityManager->AddNewEntity();
		entityManager->AddComponent<SpriteRenderer>(castle, castlePath, 32, 32, m_AssetHandler);
		Transform* castleTransform = &entityManager->GetComponent<Transform>(castle);
		castleTransform->m_Position = Map::ConvertToScreen(Map::m_Data.m_Regions[regionIndex].m_RegionCapital);
	}

	m_UIFont = m_AssetHandler->LoadFontFromFile("Assets/Fonts/TestFont.ttf");

	std::vector<unsigned int> id0{ 1, 2, 3, 4, 5, 6, 7 };
	EntityID char0 = CreateCharacter(*entityManager, id0, Title::King, "Kingdom of Milano", "Erik", 100, 5, false, sf::Color(181, 54, 107));

	std::vector<unsigned int> id1{ 8, 9, 10, 11, 12 };
	EntityID char1 = CreateCharacter(*entityManager, id1, Title::Emperor, "Roman Empire", "Robin", 100, 10, false, sf::Color(54, 181, 105));

	std::vector<unsigned int> id2{ 13, 14, 15, 16, 17 };
	EntityID char2 = CreateCharacter(*entityManager, id2, Title::King, "Kingdom of Sicilies", "Fredrik", 100, 10, true, sf::Color(200, 181, 105));

	EntityID playerUnit = entityManager->AddNewEntity();
	entityManager->AddComponent<PlayerUnit>(playerUnit, char2);
	entityManager->AddComponent<SpriteRenderer>(playerUnit, "Assets/Graphics/Soldier Unit.png", 32, 32, m_AssetHandler);

// 	std::vector<unsigned int> id3{ 4 };
// 	CreateCharacter(char3, Title::Duke, "Republic of Veneto", "Viktor", id3, 100, 10, false, sf::Color(149, 181, 54), 0);
// 
// 	std::vector<unsigned int> id4{ 5 };
// 	CreateCharacter(char4, Title::King, "Kingdom of Friuli", "Fred", id4, 100, 10, false, sf::Color(34, 51, 67), 0);
// 
// 	std::vector<unsigned int> id5{ 6 };
// 	CreateCharacter(char5, Title::Duke, "Republic of Genua", "Gustav", id5, 100, 5, false, sf::Color(60, 54, 107), 0);
// 
// 	std::vector<unsigned int> id6{ 7 };
// 	CreateCharacter(char6, Title::Count, "County of Romagna", "Emil", id6, 100, 10, false, sf::Color(54, 70, 105), 0);
// 
// 	std::vector<unsigned int> id7{ 8 };
//  CreateCharacter(char7, Title::Duke, "Republic of Firenze", "Fredrik", id7, 100, 10, false, sf::Color(88, 42, 42), 0);
// 
// 	std::vector<unsigned int> id8{ 9 };
// 	CreateCharacter(char8, Title::Baron, "Barony of Urbino", "Urban", id8, 100, 10, false, sf::Color(149, 55, 54), 0);
// 
// 	std::vector<unsigned int> id9{ 10 };
// 	CreateCharacter(char9, Title::Baron, "Barony of Umbria", "Brian", id9, 100, 10, false, sf::Color(34, 51, 120), 0);
// 
// 	std::vector<unsigned int> id10{ 11 };
// 	CreateCharacter(char10, Title::Emperor, "Roman Empire", "Robin", id10, 100, 5, false, sf::Color(90, 54, 200), 0);
// 
// 	std::vector<unsigned int> id11{ 12 };
// 	CreateCharacter(char11, Title::Count, "County of Abruzzo", "Albin", id11, 100, 10, false, sf::Color(54, 181, 50), 0);
// 
// 	std::vector<unsigned int> id12{ 13 };
// 	CreateCharacter(char12, Title::Count, "County of Campania", "Camilla", id12, 100, 10, false, sf::Color(165, 100, 42), 0);
// 
// 	std::vector<unsigned int> id13{ 14 };
// 	CreateCharacter(char13, Title::Count, "County of Bari", "Bart", id13, 100, 10, false, sf::Color(149, 181, 100), 0);
// 
// 	std::vector<unsigned int> id14{ 15 };
// 	CreateCharacter(char14, Title::Baron, "Barony of Basilicata", "Basil", id14, 100, 10, false, sf::Color(34, 100, 67), 0);
// 
// 	std::vector<unsigned int> id15{ 16 };
// 	CreateCharacter(char15, Title::Count, "County of Calabria", "Karl", id15, 100, 5, false, sf::Color(181, 200, 107), 0);
// 
// 	std::vector<unsigned int> id16{ 17 };
// 	CreateCharacter(char16, Title::King, "Kingdom of Sicily", "Sissy", id16, 100, 10, false, sf::Color(200, 181, 105), 0);

	entityManager->AddComponent<WarmindComponent>(char1, 1, char0);
	entityManager->AddComponent<WarmindComponent>(char0, 4, char1);

	//UI
	EntityID topPortraitUI = entityManager->AddNewEntity();
	entityManager->AddComponent<UISpriteRenderer>(topPortraitUI, "Assets/Graphics/Unit.png", 64, 64, m_AssetHandler);
	Transform* topPortraitUITransform = &entityManager->GetComponent<Transform>(topPortraitUI);

	EntityID bottomPortraitUI = entityManager->AddNewEntity();
	entityManager->AddComponent<UISpriteRenderer>(bottomPortraitUI, "Assets/Graphics/Unit.png", 128, 128, m_AssetHandler);
	Transform* bottomPortraitUITransform = &entityManager->GetComponent<Transform>(bottomPortraitUI);
	bottomPortraitUITransform->m_Position = Vector2D(0.0f, m_Resolution.y - 128.0f);

	EntityID windowUI = entityManager->AddNewEntity();
	entityManager->AddComponent<UIWindow>(windowUI, m_UIFont, topPortraitUI, bottomPortraitUI);

	UIWindow* windowUIComponent = &entityManager->GetComponent<UIWindow>(windowUI);
	topPortraitUITransform->m_Position = Vector2D(windowUIComponent->m_SizeX * 0.1f, windowUIComponent->m_SizeY * 0.025f);
	entityManager->SetEntityActive(topPortraitUI, false);
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

EntityID Game::CreateCharacter(EntityManager& entityManager, std::vector<unsigned int>& ownedRegions, Title title, const char* realmName, const char* characterName, int army, int gold, bool playerControlled, sf::Color color)
{
	EntityID character = entityManager.AddNewEntity();
	entityManager.AddComponent<CharacterComponent>(character, title, realmName, characterName, ownedRegions, gold, army, playerControlled, color, 0); // 0 is personality index (move away from indicies asap)

	for (int i : ownedRegions)
	{
		Map::SetRegionColor(i, color);
	}

	EntityID textUI = entityManager.AddNewEntity();
	entityManager.AddComponent<UIText>(textUI, m_UIFont, realmName, ownedRegions);

	return character;
}
