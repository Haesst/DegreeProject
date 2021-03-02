#pragma once
#include <SFML/Graphics.hpp>
#include "Engine/Vector2D.h"

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
	static Vector2D GetMousePosition();
	static int GetMouseScrollDirection();
protected:
private:
	static bool leftMouseClicked;
	static bool mouseScrollWheelChanged;
	static Vector2D mousePosition;
	static int mouseScrollDirection;
};