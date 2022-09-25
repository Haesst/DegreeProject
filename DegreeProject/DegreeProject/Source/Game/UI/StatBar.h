#pragma once

#include <SFML/Graphics.hpp>
#include <Game/Data/Types.h>
#include <Game/Data/Titles.h>
#include <Engine/Vector2D.h>
#include <Game/UI/UIWindow.h>

class StatBar : public UIWindow
{
public:

	StatBar(UIID id, sf::Font font, Vector2D position, Vector2D size);
	void start() override;
	void render() override;
	void updateOwnerColor(sf::Color& newColor);

private:

	void onDayChange();
	void updateStats();
	const char* m_PositiveSign = "+";
};