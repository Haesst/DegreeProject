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

	MapDrawer mapDrawer;
	auto map = mapDrawer.GetMap("Assets/Map/Map.txt", 'O', resolution, 30.0f);

	// sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), "GRAND STRATEGY GAME 2.5!", sf::Style::Fullscreen);
	// sf::CircleShape shape(10.f);
	// shape.setPosition(window.getSize().x * .5f, window.getSize().y * .5);
	// shape.setFillColor(sf::Color::Green);
	// sf::CircleShape redShape(10.f);
	// redShape.setPosition(window.getSize().x * .5f, window.getSize().y * .6);
	// redShape.setFillColor(sf::Color::Red);

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

		// float dt = deltaClock.restart().asSeconds();
		// float newX = shape.getPosition().x + (100 * dt);
		// float redShapeY = redShape.getPosition().y + (100 * dt);
		// 
		// if (newX > window.getSize().x - 20)
		// {
		// 	newX = 0;
		// }
		// 
		// if (redShapeY > window.getSize().y - 20)
		// {
		// 	redShapeY = 0;
		// }
		// 
		// shape.setPosition(newX, shape.getPosition().y);
		// redShape.setPosition(redShape.getPosition().x, redShapeY);

		// Update
		EntityManager::Get().Update();

		// Render
		window->clear(sf::Color::Blue);
		mapDrawer.DrawMap(&map, *window);
		EntityManager::Get().Render();
		window->display();


		/*window.clear(sf::Color::Blue);
		window.draw(Spr);
		window.draw(shape);
		window.draw(redShape);
		window.display();*/
	}

	Window::Cleanup();
	return 0;
}
