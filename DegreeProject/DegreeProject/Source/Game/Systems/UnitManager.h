#pragma once

#include <vector>
#include "Engine/Vector2D.h"
#include "Game/Data/Types.h"
#include "Game/GameDate.h"

struct Unit;
class AssetHandler;
struct Date;
struct Character;

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
	void showLinePath(Unit& unit);
	void setupUnitUI(Unit& unit);
	void updateUnitUI(Unit& unit);
	void startCombatTimer(UnitID unit, UnitID enemyUnit);
	UnitID unitAtSquare(Vector2DInt square, UnitID unitID);
	std::vector<UnitID> getAlliesAtSquare(const Character& character, Vector2DInt square);
	void determineCombat(UnitID unitID, UnitID enemyID);
	bool neutralUnitAtSquare(Character& character, Vector2DInt square);
	bool weightedRandomCombat(float weight);
	void startConquerRegion(Unit& unit);

	void updateSprite(Unit& unit);
	void displayProgressMeter(Unit& unit, float timeElapsed, float totalTime, sf::Vector2f offset, float rotation, sf::Color fillColor);
	
private:
	sf::Texture m_BoatTexture;

	Date m_LastDay;
	static UnitID m_UnitIDs;
	static UnitManager* m_Instance;
	std::vector<Unit> m_Units;

	float m_MoveTolerance = 10.f;

	Vector2D m_SeizeMeterOffset = { 0.0f, -20.0f };
	Vector2D m_CombatMeterOffset = { 32.0f, 32.0f };
	Vector2D m_SeizeMeterInnerOffset = { 0.5f, 0.5f };

	float m_ProgressMeterWidth = 32.0f;
	float m_ProgressMeterHeight = 12.0f;
	float m_ProgressMeterBorder = 1.0f;
	float m_ProgressMeterDoubleBorder = 1.0f;

	sf::Color m_ProgressMeterOuterColor = sf::Color(25, 25, 25, 250);
	sf::Color m_SeizeMeterFillColor = sf::Color(40, 150, 100, 250);
	sf::Color m_CombatMeterFillColor = sf::Color(170, 70, 40, 250);

	const unsigned int m_SpriteSize = 32;
	sf::Texture m_UnitTexture;
	sf::Sprite m_UnitSprite;
	sf::VertexArray m_UnitLinePath;

	float m_LeaderCombatMultiplier = 1.2f;
};