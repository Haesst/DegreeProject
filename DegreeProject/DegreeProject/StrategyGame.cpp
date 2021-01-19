#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "GRAND STRATEGY GAME 2.5!", sf::Style::Fullscreen);
	sf::CircleShape shape(10.f);
	shape.setPosition(window.getSize().x * .5f, window.getSize().y * .5);
	shape.setFillColor(sf::Color::Green);
	sf::CircleShape redShape(10.f);
	redShape.setPosition(window.getSize().x * .5f, window.getSize().y * .6);
	redShape.setFillColor(sf::Color::Red);

	while (window.isOpen())
	{
		float newX = shape.getPosition().x + 0.1f;
		float redShapeY = redShape.getPosition().y + 0.1f;
		
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

		window.clear();
		window.draw(shape);
		window.draw(redShape);
		window.display();
	}

	return 0;
}