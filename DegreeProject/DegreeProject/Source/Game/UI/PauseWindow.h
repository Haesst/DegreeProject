#pragma once

#include <Game/UI/UIWindow.h>

class PauseWindow : public UIWindow
{
public:

	PauseWindow(UIID id, sf::Font font, Vector2D position, Vector2D size);
	void start() override;
	void render() override;
	void closeWindow() override;
	void openWindow() override;
};
