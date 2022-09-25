#pragma once

#include <SFML/Graphics.hpp>
#include <Game/Data/UIElement.h>
#include <Engine/Vector2D.h>
#include <Game/Data/Types.h>
#include <Game/UI/UIWindow.h>

class ArmyWindow : public UIWindow
{
public:	

	ArmyWindow(UIID id, sf::Font font, Vector2D, Vector2D size);
	void start() override;
	void update() override;
	void render() override;
	void openWindow() override;
	void closeWindow() override;

private:

	void onDayChange();
	void updateInfo();
	void clickButton() override;
	CharacterID m_CurrentCharacterID = INVALID_CHARACTER_ID;
	int m_DaySubscriptionHandle = -1;
	sf::Text m_ArmyText;
	const std::string m_ArmyOf = "Army of ";
};