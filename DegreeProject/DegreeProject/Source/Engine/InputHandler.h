#pragma once
#include <SFML/Graphics.hpp>
#include "Engine/Vector2D.h"

class InputHandler
{
public:
	static void HandleInputEvents();
	static bool GetLeftMouseClicked();
	static bool GetRightMouseClicked();
	static bool GetLeftMouseReleased();
	static bool GetRightMouseReleased();
	static bool GetMouseScrollWheelChanged();
	static Vector2D GetMousePosition();
	static int GetMouseScrollDirection();
	static bool GetPlayerSelected();
	static void SetPlayerSelected(bool selected);
private:
	static bool m_LeftMouseClicked;
	static bool m_RightMouseClicked;
	static bool m_LeftMouseReleased;
	static bool m_RightMouseReleased;
	static bool m_MouseScrollWheelChanged;
	static bool m_PlayerSelected;
	static Vector2D m_MousePosition;
	static int m_MouseScrollDirection;
	static const float MAX_ZOOM;
	static const float MIN_ZOOM;
	static const float MOVE_SPEED;
	static const float ZOOM_SPEED;
};