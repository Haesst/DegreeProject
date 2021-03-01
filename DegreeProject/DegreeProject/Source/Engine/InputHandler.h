#pragma once
#include <SFML/Graphics.hpp>

class InputHandler
{
public:
	InputHandler();
	~InputHandler();
	void HandleInputEvents(sf::RenderWindow& window);
protected:
private:
};