#include <SFML/Graphics.hpp>
#include "Engine/AssetHandler.h"
#include "Engine/Time.h"
#include "Engine/Window.h"
#include "ECS/EntityManager.h"
#include "Game/MapDrawer.h"

// For ECS demo
#include "Game/Systems/ECSExampleSystem.h"
#include "Game/Components/MovingCircle.h"

int main()
{
	sf::Vector2 resolution(1920, 1080);
	AssetHandler AssetLoader;
	sf::Sprite Spr;
	sf::Texture Texture;

	Spr = AssetLoader.LoadImageFromFile("Assets/Graphics/Test.jpg", Texture);
	Spr.scale(.5f, .5f);

	sf::SoundBuffer buffer;
	sf::Sound TestSound = AssetLoader.LoadAudioFile("Assets/Audio/MenuMusic.wav", buffer);
	TestSound.setVolume(100);
	TestSound.play();

	MapDrawer mapDrawer;
	auto map = mapDrawer.GetMap("Assets/Map/Map.txt", 'O', resolution, 30.0f);

	Window::Init(sf::VideoMode(resolution.x, resolution.y), "GRAND STRATEGY  GAME 2.5!", sf::Style::Fullscreen);

	// Cache entity manager
	EntityManager* entityManager = &EntityManager::Get();
	// Cache window
	sf::RenderWindow* window = Window::GetWindow();

	// ECS example --------------------

	// Register the system
	entityManager->RegisterSystem<ECSExampleSystem>();

	// Create an entity
	EntityID dot1 = entityManager->AddNewEntity();
	// Add necessary components
	entityManager->AddComponent<MovingCircle>(dot1);

	// Get transform of entity (or any other component)
	Transform* dot1Transform = &entityManager->GetComponent<Transform>(dot1);
	dot1Transform->m_Position = { window->getSize().x * 0.5f, window->getSize().y * 0.5f };

	// Create another entity
	EntityID dot2 = entityManager->AddNewEntity();
	// Add necessary components
	entityManager->AddComponent<MovingCircle>(dot2);

	// Get transform and moving circle of entity
	Transform* dot2Transform = &entityManager->GetComponent<Transform>(dot2);
	MovingCircle* dot2Circle = &entityManager->GetComponent<MovingCircle>(dot2);
	dot2Transform->m_Position = { window->getSize().x * 0.5f, window->getSize().y * 0.6f };
	dot2Circle->m_Direction = { 0.0f, 1.0f };
	dot2Circle->m_Color = sf::Color::Yellow;

	// ECS example --------------------

	while (window->isOpen())
	{
		// Time
		Time::UpdateTime();

		// Events
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
			}
		}

		// Update
		EntityManager::Get().Update();

		// Render
		window->clear(sf::Color::Blue);
		mapDrawer.DrawMap(&map, *window);
		EntityManager::Get().Render();
		window->display();

	}

	Window::Cleanup();
	return 0;
}
