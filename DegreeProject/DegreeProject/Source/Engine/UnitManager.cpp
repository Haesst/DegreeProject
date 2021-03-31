#include "UnitManager.h"

#include "Game/Data/Unit.h"
#include "Engine/Log.h"

UnitManager* UnitManager::m_Instance = nullptr;
UnitID UnitManager::m_UnitIDs = INVALID_UNIT_ID + 1;

void UnitManager::start()
{
}

void UnitManager::update()
{
	for (auto& unit : m_Units)
	{
		// Move unit
		// Engage enemy
		// Siege
	}
}

void UnitManager::render()
{
	// Render unit
}

UnitManager& UnitManager::get()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new UnitManager();
	}

	return *m_Instance;
}

void UnitManager::addUnit(CharacterID owner)
{
	Unit newUnit;
	newUnit.m_Owner = owner;
	newUnit.m_UnitID = m_UnitIDs++;

	m_Units.push_back(newUnit);
}

Unit UnitManager::getUnitOfCharacter(CharacterID character)
{
	for (auto& unit : m_Units)
	{
		if (unit.m_Owner == character)
		{
			return unit;
		}
	}

	ASSERT(false, "Given character doesn't have any units.");
	return Unit();
}

Unit UnitManager::getUnitWithId(UnitID id)
{
	for (auto& unit : m_Units)
	{
		if (unit.m_UnitID == id)
		{
			return unit;
		}
	}

	ASSERT(false, "No unit with given ID exists");
	return Unit();
}

void UnitManager::giveUnitPath(std::vector<Vector2DInt> path)
{
}