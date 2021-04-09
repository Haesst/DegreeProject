#pragma once

#include <vector>
#include "Engine/Vector2D.h"
#include "Game/Data/Types.h"

struct Unit;
class AssetHandler;

class UnitManager
{
public:

	void start();
	void update();
	void render();

	static UnitManager& get();

	UnitID addUnit(CharacterID owner, int size);
	Unit& getUnitOfCharacter(CharacterID character);
	Unit& getUnitWithId(UnitID id);

	void raiseUnit(UnitID unitID, Vector2DInt location);
	void dismissUnit(UnitID unitID);
	void giveUnitPath(UnitID unitID, std::vector<Vector2DInt> path);

private:
	void moveUnit(Unit& unit);
	void unitCombat(Unit& unit);
	void unitSiege(Unit& unit);
	void showPath(Unit& unit);
	void startCombatTimer(UnitID unit, UnitID enemyUnit);
	UnitID unitAtSquare(Vector2DInt square, UnitID unitID);
	void determineCombat(UnitID unitID, UnitID enemyID);
	bool neutralUnitAtSquare(CharacterID character, Vector2DInt square);
	bool weightedRandomCombat(float weight);
	void startConquerRegion(Unit& unit);

	void updateSprite(Unit& unit);
	void displayProgressMeter(Unit& unit, float timeElapsed, float totalTime, sf::Vector2f offset, sf::Color fillColor);

private:
	static UnitID m_UnitIDs;
	static UnitManager* m_Instance;
	std::vector<Unit> m_Units;

	float m_MoveTolerance = 0.3f;

	Vector2D m_SeizeMeterOffset = { 0.0f, -20.0f };
	Vector2D m_CombatMeterOffset = { 0.0f, 30.0f };
	Vector2D m_SeizeMeterInnerOffset = { 1.0f, 1.0f };

	float m_ProgressMeterWidth = 32.0f;
	float m_ProgressMeterHeight = 12.0f;
	float m_ProgressMeterBorder = 1.0f;
	float m_ProgressMeterDoubleBorder = 1.0f;

	sf::Color m_ProgressMeterOuterColor = sf::Color(25, 25, 25, 250);
	sf::Color m_SeizeMeterFillColor = sf::Color(40, 70, 170, 250);
	sf::Color m_CombatMeterFillColor = sf::Color(170, 70, 40, 250);

	sf::Texture m_UnitTexture;
	sf::Sprite m_UnitSprite;
};