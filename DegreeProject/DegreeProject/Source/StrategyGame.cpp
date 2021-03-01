#include <SFML/Graphics.hpp>
#include "Engine/AssetHandler.h"
#include "ECS/EntityManager.h"

int main()
{
	AssetHandler AssetLoader;
	sf::Sprite Spr;
	sf::Texture Texture;

	Spr = AssetLoader.LoadImageFromFile("Assets/Graphics/Test.jpg", Texture);

	Spr.scale(.5f, .5f);

	sf::RenderWindow window(sf::VideoMode(1920, 1080), "GRAND STRATEGY GAME 2.5!", sf::Style::Fullscreen);
	sf::CircleShape shape(10.f);
	shape.setPosition(window.getSize().x * .5f, window.getSize().y * .5);
	shape.setFillColor(sf::Color::Green);
	sf::CircleShape redShape(10.f);
	redShape.setPosition(window.getSize().x * .5f, window.getSize().y * .6);
	redShape.setFillColor(sf::Color::Red);

	sf::Clock deltaClock;

	EntityManager* entityManager = &EntityManager::Get();

	while (window.isOpen())
	{
		float dt = deltaClock.restart().asSeconds();
		float newX = shape.getPosition().x + (100 * dt);
		float redShapeY = redShape.getPosition().y + (100 * dt);
		
		if (newX > window.getSize().x - 20)
		{
			newX = 0;
		}

		if (redShapeY > window.getSize().y - 20)
		{
			redShapeY = 0;
		}

		shape.setPosition(newX, shape.getPosition().y);
		redShape.setPosition(redShape.getPosition().x, redShapeY);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		EntityManager::Get().Update();
		EntityManager::Get().Render();

		window.clear();
		window.draw(Spr);
		window.draw(shape);
		window.draw(redShape);
		window.display();
	}

	return 0;
}
