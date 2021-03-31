#pragma once

#include <vector>
#include "Engine/Vector2D.h"
#include "Game/Data/Types.h"

struct Unit;

class UnitManager
{
public:

	void start();
	void update();
	void render();

	static UnitManager& get();

	void addUnit(CharacterID owner);
	Unit getUnitOfCharacter(CharacterID character);
	Unit getUnitWithId(UnitID id);
	void giveUnitPath(std::vector<Vector2DInt> path);

private:
	static UnitID m_UnitIDs;
	static UnitManager* m_Instance;
	std::vector<Unit> m_Units;
};