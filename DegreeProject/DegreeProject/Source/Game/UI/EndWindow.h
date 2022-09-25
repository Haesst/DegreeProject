#pragma once

#include <SFML/Graphics.hpp>
#include <Game/Data/Types.h>
#include <Engine/Vector2D.h>
#include <Game/UI/UIWindow.h>

class EndWindow : public UIWindow
{
public:

	EndWindow(UIID id, sf::Font font, Vector2D position, Vector2D size);
	void start() override;
	void update() override;
	void render() override;
	void openWindow(unsigned int loseCause);

private:

	void clickButton() override;
	std::vector<const char*> m_EndStrings = { "Your\nRoyal\nLine\nEnds\nHere", "Your\nRealm\nWas\nConquered", "The\nWorld\nIs\nYours" };
	unsigned int m_EndStringIndex = 2;
};
