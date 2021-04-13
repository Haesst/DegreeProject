#pragma once

#include "Game/Data/Types.h"
#include <SFML/Graphics.hpp>
#include "Engine/Vector2D.h"
#include "Game/Data/Titles.h"
#include "Game/Data/UIType.h"

class WarIcon
{
public:

	WarIcon(UIID ID, sf::Font font, CharacterID attackerID, CharacterID defenderID, UIType type);
	void update();
	void render();
};