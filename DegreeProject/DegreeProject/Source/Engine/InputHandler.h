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
	static bool m_LeftMouseClicked;
	static bool m_MouseScrollWheelChanged;
	static Vector2D m_MousePosition;
	static int m_MouseScrollDirection;
	static const float MAX_ZOOM;
	static const float MIN_ZOOM;
	static const float MOVE_SPEED;
	static const float ZOOM_SPEED;
};