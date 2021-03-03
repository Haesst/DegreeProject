#include "Game.h"

#include "Engine/Window.h"
#include "Engine/Time.h"
#include "Engine/InputHandler.h"
#include "Game/MapDrawer.h"
#include "ECS/EntityManager.h"
#include <Game/Components/MovingCircle.h>
#include <Game/Components/SpriteRenderer.h>
#include <Game/Components/Map.h>
#include <Game/Systems/PlayerSystem.h>
#include <Game/Systems/ECSExampleSystem.h>
#include <Game/Systems/SpriteRenderSystem.h>
#include <Game/Systems/MapSystem.h>

Game::~Game()
{
	delete m_MapDrawer;
	delete m_AssetHandler;
}

void Game::Init()
{
	InitWindow();
	InitAssets();
	InitMap();
	InitSystems();
	AddEntitys();
}

void Game::Run()
{
	sf::RenderWindow* internalWindow = m_Window->GetWindow();
	sf::View view(sf::Vector2(960.0f, 540.0f), sf::Vector2(1920.0f, 1080.0f));
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
		internalWindow->clear(sf::Color::Blue);
		// m_MapDrawer->DrawMap(&m_Map, *internalWindow);
		EntityManager::Get().Render();
		internalWindow->display();
	}

	m_Window->Cleanup();
}

void Game::InitWindow()
{
	Window::Init(sf::VideoMode(m_Resolution.x, m_Resolution.y), m_GameTitle, sf::Style::Default);
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
}

void Game::AddEntitys()
{
	EntityManager* entityManager = &EntityManager::Get();
	// Create an entity
	EntityID dot1 = entityManager->AddNewEntity();
	// Add necessary components
	entityManager->AddComponent<MovingCircle>(dot1);
	entityManager->AddComponent<SpriteRenderer>(dot1, "Assets/Graphics/Charizard.png", 32, 32, m_AssetHandler);

	// Get transform of entity (or any other component)
	Transform* dot1Transform = &entityManager->GetComponent<Transform>(dot1);
	dot1Transform->m_Position = { m_Window->GetWindow()->getSize().x * 0.5f, m_Window->GetWindow()->getSize().y * 0.5f };

	// Create another entity
	EntityID dot2 = entityManager->AddNewEntity();
	entityManager->AddComponent<SpriteRenderer>(dot2, "Assets/Graphics/Test.jpg", 32, 32, m_AssetHandler);
	// Add necessary components
	entityManager->AddComponent<MovingCircle>(dot2);

	//Create Player Dot
	EntityID playerDot = entityManager->AddNewEntity();
	entityManager->AddComponent<Player>(playerDot);
	Transform* playerDotTransform = &entityManager->GetComponent<Transform>(playerDot);
	Player* playerDotCircle = &entityManager->GetComponent<Player>(playerDot);
	playerDotTransform->m_Position = { m_Window->GetWindow()->getSize().x * 0.5f, m_Window->GetWindow()->getSize().y * 0.6f };
	playerDotCircle->m_Direction = { 0.0f, 0.0f };
	playerDotCircle->m_Color = sf::Color::Black;

	// Get transform and moving circle of entity
	Transform* dot2Transform = &entityManager->GetComponent<Transform>(dot2);
	MovingCircle* dot2Circle = &entityManager->GetComponent<MovingCircle>(dot2);
	dot2Transform->m_Position = { m_Window->GetWindow()->getSize().x * 0.5f, m_Window->GetWindow()->getSize().y * 0.6f };
	dot2Circle->m_Direction = { 0.0f, 1.0f };
	dot2Circle->m_Color = sf::Color::Yellow;

	EntityID map = entityManager->AddNewEntity();
	entityManager->AddComponent<Map>(map, m_AssetHandler->GetTextureAtPath("Assets/Graphics/TileSet.png"));
}
