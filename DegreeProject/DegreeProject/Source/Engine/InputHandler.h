#pragma once
#include "Engine/Vector2D.h"

enum Inputs
{
	LeftMouseClicked,
	RightMouseClicked,
	LeftMouseReleased,
	RightMouseReleased,
	MiddleMouseClicked,
	MiddleMouseReleased,
	MouseMoved,
	MouseScrolled,
	EscapePressed,
	EnterPressed,
	EnterReleased,
	BackSpacePressed,
	BackSpaceReleased,
	CharacterWindowOpen,
	RegionWindowOpen,
	WarWindowOpen,
	FamilyTreeWindowOpen,
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
	static void setLeftMouseClicked(bool clicked);
	static void setRightMouseClicked(bool clicked);
	static void setLeftMouseReleased(bool released);
	static void setRightMouseReleased(bool released);
	static Vector2D getMousePosition();
	static Vector2D getUIMousePosition();
	static Vector2DInt getMouseMapPosition();
	static float getMouseScrollDirection();
	static bool getMouseScrolled();
	static bool getPlayerUnitSelected();
	static bool getMouseMoved();
	static bool getEscapePressed();
	static bool getCharacterWindowOpen();
	static bool getRegionWindowOpen();
	static bool getWarWindowOpen();
	static bool getFamilyTreeWindowOpen();
	static void setPlayerUnitSelected(bool selected);
	static void setCharacterWindowOpen(bool open);
	static void setWarWindowOpen(bool open);
	static void setRegionWindowOpen(bool open);
	static void setFamilyTreeWindowOpen(bool open);
	static float getZoomSpeed();
	static void setUIView(sf::View& uiView);

	static bool m_Inputs[Inputs::PlayerUnitSelected + 1];
	static float m_TotalZoom;
	static float m_InverseZoom;
private:
	static void moveView(sf::RenderWindow& window, sf::View& view);
	static void setMousePosition(int xPosition, int yPosition, const sf::RenderWindow& window);
	static bool allowedToZoomView(const sf::View& view);
	static void zoomView(sf::RenderWindow& window, sf::View& view);

private:
	static sf::View& m_UIView;

	static float maxCenterXPosition;
	static float minCenterXPosition;

	static float maxCenterYPosition;
	static float minCenterYPosition;
};