#pragma once
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
	static Vector2DInt GetMouseMapPosition();
	static int GetMouseScrollDirection();
	static bool GetPlayerUnitSelected();
	static bool GetMouseMoved();
	static void SetPlayerUnitSelected(bool selected);
	static float GetZoomSpeed();
};