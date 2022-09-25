#pragma once

#include <SFML/Graphics.hpp>
#include <Game/Data/Types.h>
#include <Engine/Vector2D.h>
#include <Game/UI/UIWindow.h>

class DateBar : public UIWindow
{
public:

	DateBar(UIID id, sf::Font font, Vector2D position, Vector2D size);
	void start() override;
	void update() override;
	void render() override;
	void updateOwnerColor(sf::Color& newColor);

private:

	void onDayChange();
	void updateStats();
	void clickButton() override;
	void updateSpeedShapes();	
	const unsigned int m_NumberOfSpeeds = 5;
	const unsigned int m_NumberOfButtons = 3;
	sf::Color m_PauseSpeedColor = sf::Color::Red;
	sf::Color m_PlaySpeedColor = sf::Color::Green;
	float m_ButtonThickness = 10.0f;
	unsigned int m_CurrentSpeedLevel = 3;
	int m_DaySubscriptionHandle = 0;
	bool m_Paused = false;
};