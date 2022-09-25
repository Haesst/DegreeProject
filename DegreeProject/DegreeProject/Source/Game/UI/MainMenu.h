#pragma once

#include <Game/UI/UIWindow.h>

class MainMenu : public UIWindow
{
public:

	MainMenu(UIID id, sf::Font font, Vector2D, Vector2D size);
	void start() override;
	void update() override;
	void render() override;

private:

	void openWindow() override;
	void closeWindow() override;
	void clickButton() override;
	void handleWindow() override;
	bool m_WasPaused;
};
