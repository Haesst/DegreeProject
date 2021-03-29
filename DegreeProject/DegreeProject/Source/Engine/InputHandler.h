#pragma once
#include "Engine/Vector2D.h"

enum Inputs
{
	LeftMouseClicked,
	RightMouseClicked,
	LeftMouseReleased,
	RightMouseReleased,
	MiddleMouseClicked,
	MouseMoved,
	MouseScrolled,
	EscapePressed,
	CharacterWindowOpen,
	RegionWindowOpen,
	PlayerUnitSelected
};

class InputHandler
{
public:
	static void HandleInputEvents();
	static bool GetLeftMouseClicked();
	static bool GetRightMouseClicked();
	static bool GetLeftMouseReleased();
	static bool GetRightMouseReleased();
	static Vector2D GetMousePosition();
	static Vector2DInt GetMouseMapPosition();
	static float GetMouseScrollDirection();
	static bool GetMouseScrolled();
	static bool GetPlayerUnitSelected();
	static bool GetMouseMoved();
	static bool GetEscapePressed();
	static bool GetCharacterWindowOpen();
	static bool GetRegionWindowOpen();
	static void SetPlayerUnitSelected(bool selected);
	static void SetCharacterWindowOpen(bool open);
	static void SetRegionWindowOpen(bool open);
	static float GetZoomSpeed();
private:
	static void MoveView(sf::RenderWindow* window, sf::View& view);
	static void SetMousePosition(int xPosition, int yPosition, sf::RenderWindow* window);
	static bool AllowedToZoomView(sf::View& view);
	static void ZoomView(sf::RenderWindow* window, sf::View& view);
	static bool AllowedToMoveView(sf::View& view);
};