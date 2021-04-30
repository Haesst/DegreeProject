#pragma once

#include <SFML/Graphics.hpp>

#include "Game/Map/Map.h"
#include "Game/Data/Types.h"
#include "Game/Data/Unit.h"
#include "Game/Pathfinding.h"

#include "Engine/Vector2D.h"
#include "Engine/InputHandler.h"
#include "Engine/Window.h"
#include "Game/Systems/UnitManager.h"

enum class loseCause
{
	No_Heir,
	Unlanded
};

class Player
{
public:
	Player(CharacterID ownedCharacter);

	void update();
	void render();

	void loseGame(loseCause cause);
	void winGame();

private:
	void hoverOverRegion();
	void clickDrag();
	void selectUnit(UnitID id);
	void tryToSelectUnit();
	void moveUnit();
	void deselectUnits();

private:
	bool m_Draging = false;
	sf::RectangleShape m_DragWindow;

	bool m_MouseDownLastFrame = false;
	Vector2D m_MousePosition;
	Vector2D m_DragDirection = { 0.0f, 0.0f };

	int m_HighlightedRegion = -1;
	unsigned int m_SelectedUnitID = INVALID_UNIT_ID;
	unsigned int m_OwnedCharacter = INVALID_CHARACTER_ID;
};