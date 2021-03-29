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
	static void handleInputEvents();
	static bool getLeftMouseClicked();
	static bool getRightMouseClicked();
	static bool getLeftMouseReleased();
	static bool getRightMouseReleased();
	static Vector2D getMousePosition();
	static Vector2DInt getMouseMapPosition();
	static float getMouseScrollDirection();
	static bool getMouseScrolled();
	static bool getPlayerUnitSelected();
	static bool getMouseMoved();
	static bool getEscapePressed();
	static bool getCharacterWindowOpen();
	static bool getRegionWindowOpen();
	static void setPlayerUnitSelected(bool selected);
	static void setCharacterWindowOpen(bool open);
	static void setRegionWindowOpen(bool open);
	static float getZoomSpeed();
private:
	static void moveView(sf::RenderWindow* window, sf::View& view);
	static void setMousePosition(int xPosition, int yPosition, sf::RenderWindow* window);
	static bool allowedToZoomView(sf::View& view);
	static void zoomView(sf::RenderWindow* window, sf::View& view);
	static bool allowedToMoveView(sf::View& view);
};