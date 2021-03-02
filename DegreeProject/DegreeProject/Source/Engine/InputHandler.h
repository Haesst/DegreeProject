#pragma once
#include <SFML/Graphics.hpp>

class InputHandler
{
public:
	InputHandler();
	~InputHandler();
	static void Initialize();
	static void HandleInputEvents();
	static bool GetLeftMouseClicked();
	static bool GetMouseScrollWheelChanged();
	static void SetMouseScrollWheelChanged(bool changed);
	static sf::Vector2<float> GetMousePosition();
	static int GetMouseScrollDirection();
protected:
private:
	static bool leftMouseClicked;
	static bool mouseScrollWheelChanged;
	static sf::Vector2<float> mousePosition;
	static int mouseScrollDirection;
};